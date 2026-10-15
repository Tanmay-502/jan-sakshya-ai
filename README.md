# JAN-SAKSHYA AI
### Every Complaint. Every Action. Every Trace.

JAN-SAKSHYA AI is a proposed AI-assisted civic grievance accountability system for the **AI Synergy Hackathon 2026 — Governance: “One Complaint, Five Departments.”**

It aims to connect potentially related complaints, recommend routing to municipal departments, record field-inspection evidence, and flag possible discrepancies between a department's resolution claim and later observations. The system supports accountable human review; it is not designed to silently reject or automatically close complaints.

> **Prototype status:** This repository is a starter implementation and feasibility scaffold. The complete web application, AI evaluation, physical ESP32/Raspberry Pi integration, and end-to-end tests are not yet complete. Features described as planned should not be interpreted as deployed or validated.

## Problem

Municipal grievances can arrive from several channels, use different languages, describe the same incident in different ways, and be reassigned across departments. Generic status labels also make it hard for residents to understand what is actually happening. The case study's simulated evidence reports that 23% of grievances were reassigned at least once; this statistic does not, by itself, establish the cause.

## Proposed MVP

1. Register a grievance and create a stable complaint identifier.
2. Suggest a category, potentially related complaints, and a responsible department.
3. Keep original complaint records intact; staff review uncertain incident links and assignments.
4. Let authorised field staff start an inspection using an ESP32 physical controller.
5. Capture photos and written observations through a staff phone/web interface.
6. Flag potential conflicts between resolution claims and later inspection records.
7. Give an authorised official the final decision and preserve a traceable audit history.
8. Keep eligible local inspection events queued during connection loss, then synchronise when connectivity returns.

## Proposed architecture

- **Web interface:** React (planned)
- **API/backend:** Python FastAPI
- **Central data store:** PostgreSQL (planned for a fuller deployment)
- **Local edge service:** Raspberry Pi + SQLite
- **Inspection controller:** ESP32, Wi-Fi, three buttons and buzzer
- **Evidence capture:** staff phone camera and a web form
- **Arduino:** optional for initial circuit testing; not required in the final architecture

### Hardware controls (proposed)

| Control | Purpose |
|---|---|
| Button 1 | Start inspection |
| Button 2 | Acknowledge evidence submission |
| Button 3 | Flag for human review |
| Buzzer | Non-punitive action/status acknowledgement |

Proposed GPIO mapping: buttons on GPIO 18, 19 and 21; buzzer control on GPIO 23, subject to the exact board, electrical limits and a suitable driver circuit. Verify the pinout and buzzer current/voltage before wiring.

**Important:** Buttons and a buzzer do not detect infrastructure faults. In the MVP, photos and written observations are entered via a phone/web app. Physical sensing would require suitable additional sensors and validation. A photograph alone is not definitive proof of repair or misconduct.

## Human oversight and governance

- AI outputs are recommendations, not official decisions.
- No complaint is automatically rejected or closed solely because of an AI score.
- Potential duplicates are suggested for review; original submissions and histories remain intact.
- Safety-sensitive or ambiguous cases are escalated to authorised staff.
- Corrections, reassignment reasons, inspection evidence and decisions should be auditable.
- Collect only necessary data; restrict access to sensitive photos and location details.

## Repository structure

```
.
├── README.md
├── docs/
│   ├── TECHNICAL_FEASIBILITY.md
│   ├── ARCHITECTURE.md
│   └── IMPLEMENTATION_PLAN.md
├── backend/
│   ├── README.md
│   ├── requirements.txt
│   ├── .env.example
│   └── app/
│       └── main.py
├── firmware/
│   └── esp32_inspection_controller/
│       ├── README.md
│       └── esp32_inspection_controller.ino
└── tests/
    └── README.md
```

## Getting started

### Backend starter (Python 3.10+ recommended)

1. Create a virtual environment.
2. Install packages listed in `backend/requirements.txt`.
3. Copy `backend/.env.example` to `.env` in the backend working directory and set a non-default device key.
4. Run the FastAPI development server as described in `backend/README.md`.
5. Use the API docs and sample requests to check the starter endpoints.

These instructions apply to the backend scaffold only. They do not imply that the full web app, AI model integration, Raspberry Pi offline sync or physical hardware has been implemented.

## Validation plan

The proposed acceptance tests include:
- Valid grievance creates a unique ID and timestamp.
- English/Hindi/Hinglish test cases are accepted or return an explicit uncertainty state.
- Related and unrelated complaints are tested against a manually labelled set.
- Staff can confirm or correct routing and incident-link suggestions.
- An authorised ESP32 start event is acknowledged by the backend.
- Evidence records retain complaint/inspection IDs and timestamps.
- Conflicting resolution/inspection records are flagged for human review without automatic closure.
- Local queued events synchronise without creating duplicates.
- Unauthorised device operations fail safely.
- Citizen status text is generated only from recorded workflow events.

Do not report AI accuracy or hardware integration as successful until measured with documented tests.

## Implementation priorities

1. Stabilise the API and data model.
2. Add complaint registration, review and audit screens.
3. Implement incident-link and department suggestions with explicit uncertainty.
4. Connect the ESP32 to an authenticated inspection-event endpoint.
5. Add Raspberry Pi local storage/queue and retry handling.
6. Add photo/observation upload and resolution-discrepancy review.
7. Run acceptance tests and publish measured results.

See the files in `docs/` for the technical feasibility assessment and implementation plan.

## Team

AI Synergy Hackathon 2026 — Team Git Squad.

## Disclaimer

This is a hackathon prototype proposal, not a production municipal system. Use simulated data for development unless the team has explicit permission to process real complaints or photographs. Never commit API keys, credentials, personal information, or unredacted citizen evidence.
