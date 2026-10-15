# MVP Implementation Plan

This is a proposed delivery sequence, not a claim that all phases are complete.

## Phase 1 — Run starter API
- Create a Python virtual environment.
- Install `backend/requirements.txt`.
- Configure a non-default `DEVICE_API_KEY`.
- Start the FastAPI service and check `/health` and `/docs`.
- Test accepted and rejected event requests.

**Acceptance:** API runs locally, accepted events persist, invalid device key is rejected, and repeated event IDs do not create duplicate records (verify with the actual implementation).

## Phase 2 — Assemble ESP32 controller
- Confirm exact ESP32 board pinout and buzzer electrical specs.
- Wire three buttons to the proposed GPIO pins and ground using internal pull-ups.
- Use a safe buzzer driver where needed.
- Upload the firmware sketch, configure Wi-Fi and API endpoint, and inspect serial logs.

**Acceptance:** All buttons are debounced and send distinct events; buzzer activates only on the defined acknowledgement path; no unsafe GPIO load is used.

## Phase 3 — Raspberry Pi local hosting
- Install supported Python version on Raspberry Pi.
- Deploy FastAPI locally with SQLite.
- Configure a private test network and device key.
- Test API restart and local persistence.

**Acceptance:** A test inspection event remains available after service restart. Internet interruption behavior should be tested separately; durable offline queueing/synchronisation is a future requirement until implemented.

## Phase 4 — Complaint and evidence flow
- Add simulated complaints and a staff UI.
- Add a form for observation text and photo references.
- Associate each inspection with complaint ID, inspection ID, timestamp and authorised actor.
- Add a review screen for conflicting resolution claims and observations.

**Acceptance:** A simulated water-leak complaint can be registered, inspected, and flagged for human review when a defined conflicting later observation is added.

## Phase 5 — AI assistance
- Start with configurable routing rules and text similarity as a baseline.
- Build a manually labelled set of related/unrelated complaint pairs, including Hindi/Hinglish examples.
- Measure incident-link precision/recall and department recommendation accuracy.
- Show uncertainty and permit human correction.

**Acceptance:** Report actual measured outcomes and known failure modes; do not assume target accuracy before testing.

## Phase 6 — Governance and demo rehearsal
- Verify no AI output can close or reject a complaint.
- Verify source records and audit history remain intact after corrections.
- Test API failure, incorrect key, retries, missing photos and conflicting records.
- Use fictional test data and remove any real secrets/personal information from the repository.

**Acceptance:** Deliver a repeatable end-to-end demo and a short test report showing passed, failed and not-yet-tested items.
