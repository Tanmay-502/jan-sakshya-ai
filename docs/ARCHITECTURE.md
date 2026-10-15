# Proposed System Architecture

## Components

- **Staff phone/web UI (planned):** select a complaint, submit inspection notes and photo evidence, view acknowledgement.
- **FastAPI backend (starter exists):** validate device events, store event records, expose health/API documentation.
- **SQLite local event database (starter exists):** local prototype persistence for events that successfully reach the API.
- **Raspberry Pi (planned deployment target):** host the local API and storage on-site where feasible.
- **ESP32 firmware scaffold:** three physical buttons and buzzer; sends events over Wi-Fi.
- **AI analysis (planned):** category, incident-link and department suggestions, with uncertainty and human correction.
- **Authorised reviewer UI (planned):** examine supporting records, correct links/assignments, and decide resolution status.

## Event flow

1. Staff selects/receives an authorised complaint reference.
2. Start Inspection button triggers an ESP32 event.
3. Backend validates device credentials and the event payload.
4. Backend stores the event and returns acknowledgement.
5. Buzzer gives a confirmation only after successful acknowledgement.
6. Staff phone submits photo/observation tied to the complaint and inspection.
7. Comparison logic flags a possible conflict with a recorded resolution claim.
8. Authorised official reviews the source records and records a decision.

## Design rules

- The inspection button does not prove that the inspector is physically at the reported location.
- A phone photograph does not independently prove repair or misconduct.
- A missing inspection event is not evidence that a complaint was resolved.
- AI suggestions do not automatically reject, close, reopen or prioritise a complaint.
- Preserve each original complaint and its status/history even when it is linked to another incident.
- Any location verification, offline sync, staff identity controls and production security require additional implementation and tests.

## Current implementation boundary

The starter repository contains a basic FastAPI endpoint and ESP32 sketch scaffold. A complete UI, AI matching engine, photo upload, durable offline synchronization, Raspberry Pi deployment, and hardware end-to-end test are not complete yet.
