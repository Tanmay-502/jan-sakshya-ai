"""JAN-SAKSHYA AI minimal inspection-event API starter.

For local hackathon testing only. Not production-ready.
"""
from __future__ import annotations

import os
import sqlite3
from contextlib import closing
from datetime import datetime, timezone
from pathlib import Path
from typing import Literal
from uuid import uuid4

from fastapi import Depends, FastAPI, Header, HTTPException
from pydantic import BaseModel, Field

APP_DIR = Path(__file__).resolve().parent
DB_PATH = Path(os.getenv("JAN_SAKSHYA_DB_PATH", str(APP_DIR.parent / "grievance_events.db")))
DEVICE_API_KEY = os.getenv("DEVICE_API_KEY", "")

app = FastAPI(
    title="JAN-SAKSHYA AI — Inspection Event API",
    description="Prototype API for ESP32-assisted civic inspection events.",
    version="0.1.0",
)


class InspectionEventIn(BaseModel):
    event_id: str = Field(default_factory=lambda: str(uuid4()), min_length=8, max_length=80)
    device_id: str = Field(min_length=1, max_length=80)
    complaint_id: str = Field(min_length=1, max_length=80)
    event_type: Literal["inspection_started", "evidence_acknowledged", "flag_for_review"]
    client_timestamp: str | None = Field(default=None, max_length=80)


def connect_db() -> sqlite3.Connection:
    DB_PATH.parent.mkdir(parents=True, exist_ok=True)
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    return conn


def init_db() -> None:
    with closing(connect_db()) as conn:
        conn.execute(
            """
            CREATE TABLE IF NOT EXISTS inspection_events (
                event_id TEXT PRIMARY KEY,
                device_id TEXT NOT NULL,
                complaint_id TEXT NOT NULL,
                event_type TEXT NOT NULL,
                client_timestamp TEXT,
                server_timestamp TEXT NOT NULL
            )
            """
        )
        conn.commit()


@app.on_event("startup")
def startup() -> None:
    init_db()


def require_device_key(x_device_key: str | None = Header(default=None)) -> None:
    if not DEVICE_API_KEY:
        raise HTTPException(
            status_code=503,
            detail="Device API is not configured. Set DEVICE_API_KEY before accepting events.",
        )
    if x_device_key is None or x_device_key != DEVICE_API_KEY:
        raise HTTPException(status_code=401, detail="Invalid or missing device key")


@app.get("/health")
def health() -> dict[str, str]:
    return {"status": "ok", "service": "jan-sakshya-inspection-api"}


@app.post("/api/inspection-events", status_code=201, dependencies=[Depends(require_device_key)])
def create_inspection_event(event: InspectionEventIn) -> dict[str, object]:
    server_timestamp = datetime.now(timezone.utc).isoformat()
    try:
        with closing(connect_db()) as conn:
            conn.execute(
                """
                INSERT INTO inspection_events
                (event_id, device_id, complaint_id, event_type, client_timestamp, server_timestamp)
                VALUES (?, ?, ?, ?, ?, ?)
                """,
                (
                    event.event_id,
                    event.device_id,
                    event.complaint_id,
                    event.event_type,
                    event.client_timestamp,
                    server_timestamp,
                ),
            )
            conn.commit()
    except sqlite3.IntegrityError:
        # A repeated event ID is a duplicate request, not a new event.
        with closing(connect_db()) as conn:
            row = conn.execute(
                "SELECT event_id, device_id, complaint_id, event_type, server_timestamp "
                "FROM inspection_events WHERE event_id = ?",
                (event.event_id,),
            ).fetchone()
        return {"accepted": True, "duplicate": True, "event": dict(row) if row else None}
    except sqlite3.Error as exc:
        raise HTTPException(status_code=500, detail="Unable to store inspection event") from exc

    return {
        "accepted": True,
        "duplicate": False,
        "event": {
            "event_id": event.event_id,
            "device_id": event.device_id,
            "complaint_id": event.complaint_id,
            "event_type": event.event_type,
            "server_timestamp": server_timestamp,
        },
    }


@app.get("/api/inspection-events", dependencies=[Depends(require_device_key)])
def list_inspection_events(limit: int = 50) -> dict[str, object]:
    if not 1 <= limit <= 200:
        raise HTTPException(status_code=422, detail="limit must be between 1 and 200")
    with closing(connect_db()) as conn:
        rows = conn.execute(
            "SELECT event_id, device_id, complaint_id, event_type, "
            "client_timestamp, server_timestamp FROM inspection_events "
            "ORDER BY server_timestamp DESC LIMIT ?",
            (limit,),
        ).fetchall()
    return {"events": [dict(row) for row in rows], "count": len(rows)}
