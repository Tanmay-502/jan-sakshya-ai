# Backend starter

This is the API scaffold for the JAN-SAKSHYA AI inspection-event proof of concept. It is not the complete application.

## Run locally

Use Python 3.10+ if available.

```bash
cd backend
python -m venv .venv
```

Activate the environment, then:

```bash
pip install -r requirements.txt
```

Set a device key before starting the service.

macOS/Linux:
```bash
export DEVICE_API_KEY='replace-with-a-long-random-demo-key'
uvicorn app.main:app --host 0.0.0.0 --port 8000
```

Windows PowerShell:
```powershell
$env:DEVICE_API_KEY='replace-with-a-long-random-demo-key'
uvicorn app.main:app --host 0.0.0.0 --port 8000
```

Open `/health` to check service availability and `/docs` for interactive API documentation.

## Important

- Never commit your actual device key.
- Run only on a trusted development network; do not expose the starter API to the public internet without hardening.
- Use simulated complaint identifiers and fictional data.
- Raspberry Pi deployment, photo uploads, AI analysis, full role-based authorisation, and offline sync are not complete in this starter.
