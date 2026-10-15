# MVP verification checklist

Record actual outcomes and dates during testing. Do not mark an item passed until you have run it.

- [ ] API health endpoint returns success.
- [ ] Valid device event is accepted and stored.
- [ ] Incorrect device key is rejected.
- [ ] Replaying a stable event ID does not create duplicate records.
- [ ] Each ESP32 button produces the intended event.
- [ ] Buzzer is driven within electrical limits and gives acknowledgement only after successful request.
- [ ] A phone-submitted observation can be linked to a complaint and inspection ID (UI/backend feature still required).
- [ ] A predefined resolution conflict is flagged for human review (comparison UI/logic still required).
- [ ] No AI response automatically rejects or closes a complaint.
- [ ] Test related/unrelated complaint pairs using a manually labelled set.
- [ ] Offline queue/synchronisation works without loss or duplicate writes (not implemented in current scaffold).
