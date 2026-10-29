# SoPath assignment auditor lifecycle regression

The tests verify that copy assignment stops auditing the old route, audits the
new route exactly once and unregisters auditors before destruction.
Self-assignment preserves the route, and copy construction creates an
independent auditor without damaging the source when the copy is destroyed.
