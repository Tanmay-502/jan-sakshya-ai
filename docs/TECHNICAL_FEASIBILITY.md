# Technical Feasibility — JAN-SAKSHYA AI

**Hackathon:** AI Synergy Hackathon 2026  
**Case:** Governance — One Complaint, Five Departments  
**Status:** Proposed MVP; components require integration and testing.

## 1. Feasibility summary

The proposed MVP is feasible as a scoped hackathon proof of concept using simulated grievance records, a local API, basic device events, and staff-reviewed evidence records. The full municipal platform is not yet implemented.

| Capability | Proposed approach | Feasibility / current status |
|---|---|---|
| Inspection event controller | ESP32 + buttons + buzzer | Feasible; firmware scaffold exists; physical wiring/testing pending |
| Local API and event store | FastAPI + SQLite on laptop/Raspberry Pi | Starter implementation exists; deployment and security review pending |
| Field evidence | Staff phone captures photo and observation through a web form | Feasible; web upload UI pending |
| Incident linking and routing | Rules/text similarity initially; evaluate on labelled examples | Planned; not yet implemented or evaluated |
| Resolution discrepancy flags | Compare structured resolution and later observation records | Feasible for defined MVP scenarios; review logic and UI pending |
| Offline queue and sync | Local event IDs and durable queue, idempotent sync | Planned; not yet implemented |
| Complete dashboards | React staff/citizen interfaces | Planned; not included in starter scaffold |

## 2. Proposed architecture

1. An authorised staff member selects a complaint in the web application.
2. The ESP32 sends a device event over Wi-Fi to a local FastAPI API.
3. The API validates the device key, validates the event fields, and stores accepted events in SQLite.
4. Staff submit photos and written observations using a phone-based web form. Each record references a complaint and inspection ID.
5. Comparison logic flags a possible inconsistency between a recorded department resolution claim and a later observation.
6. An authorised official reviews supporting records and chooses the next action.
7. Material actions and corrections are recorded for audit; citizen-facing status is derived from confirmed workflow records.

The Raspberry Pi may host the local API and SQLite database. During development, the API may be run on a laptop. Arduino is optional for initial circuit testing because the ESP32 can handle buttons and Wi-Fi.

## 3. Hardware and safety

Proposed controls:
- GPIO 18: Start Inspection button
- GPIO 19: Evidence Submission Acknowledged button
- GPIO 21: Flag for Review button
- GPIO 23: buzzer control, only after confirming the exact board and buzzer electrical requirements

Buttons can use internal pull-ups with the other terminal connected to ground. Verify the chosen board's pinout. If the buzzer load is not suitable for direct GPIO drive, use an appropriate transistor/driver and power arrangement. Avoid driving a buzzer directly from a GPIO when its current requirement exceeds the pin's safe limits.

The buzzer should provide non-punitive confirmation only after the backend/local service acknowledges an action. A button press alone must not be treated as proof that a photo was saved or that a civic issue was resolved.

The proposed initial evidence interface uses a staff phone camera. The ESP32 controller itself does not capture photos and does not detect road damage, water leakage or other infrastructure faults. Such sensing would require fit-for-purpose additional sensors and validation.

## 4. AI scope and accountability

Planned AI functions:
- Suggest complaint category from English, Hindi or Hinglish text.
- Suggest potentially related complaints using text, time and location fields.
- Recommend a department with a human-readable reason.
- Flag possible conflict between resolution claims and later inspection observations.
- Draft status explanations from confirmed workflow data.

AI output remains advisory. Uncertain matches, safety-sensitive cases, conflicting evidence, complaint closure/reopening and final assignment decisions require authorised human review. Original complaint records must be preserved; a proposed duplicate link must not silently delete, merge or close a complaint.

## 5. Data, privacy and security constraints

For the hackathon, use simulated complaints and fictional locations. Do not commit API keys, real citizen photos, phone numbers, exact addresses or other personal data to this repository.

Before any wider deployment:
- use authenticated identities and role-based access;
- protect transport and stored evidence;
- restrict access to photos and location details;
- validate and audit device events;
- add rate limits and robust request validation;
- design retention/deletion policies;
- define a safe correction and appeal path;
- review the system with relevant municipal stakeholders.

The current API-key check is a prototype safeguard and is not production-grade device identity management.

## 6. Failure handling required for the MVP

- **API/device disconnected:** show an error or pending state; do not signal a successful registration.
- **Repeated event submission:** use stable event IDs and idempotent handling to avoid duplicate inspection records.
- **Evidence upload fails:** keep the record pending and allow a controlled retry.
- **AI is unavailable or uncertain:** allow manual category/routing review and preserve uncertainty.
- **Conflicting records:** flag for review and preserve all source records.
- **Local server unavailable:** provide an authorised manual inspection fallback and record the interruption where possible.
- **Missing evidence:** never infer that an issue is resolved just because evidence is absent.

Offline queue/synchronisation must not be claimed complete until implemented and tested.

## 7. Acceptance tests

1. Health endpoint responds.
2. A valid device event is stored with an event ID, complaint ID, type and timestamp.
3. Replaying the same event ID does not create a duplicate record.
4. An incorrect device key is denied.
5. Each physical button produces the intended event after acknowledgement.
6. A test resolution claim and conflicting later observation create a review flag.
7. A reviewer can record a decision without deleting previous observations.
8. Status text matches confirmed workflow events.
9. Related and unrelated complaint examples are evaluated using a manually labelled test set.
10. If offline sync is implemented, interruption/recovery tests verify that queued records are not lost or duplicated.

Record actual results, errors, and limitations. Do not report an accuracy percentage or mark hardware integration as passed without test evidence.

## 8. Implementation priorities

1. Run and test the FastAPI starter locally.
2. Wire and test ESP32 buttons and buzzer safely.
3. Connect device events to the API and verify acknowledgements.
4. Deploy the local service to Raspberry Pi if time allows.
5. Add simulated complaint records, an evidence form and an audit/review view.
6. Implement simple incident linking and department rules, then measure them against labelled examples.
7. Implement the resolution-conflict scenario and test failure handling.
8. Document demo results and remaining limitations.

## 9. Scope exclusions

Not in the initial MVP: production municipal integration, full social media ingestion, automatic complaint closure/rejection, autonomous emergency dispatch, validated universal infrastructure-fault detection, or claims of proven AI accuracy. These require permissions, appropriate data, independent validation and additional engineering.
