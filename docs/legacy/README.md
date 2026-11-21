# Historical documentation catalog

This directory indexes older Coin reports and proposals without changing their original text. The catalogs were prepared on 21 November 2026. A historical entry is not evidence that a bug still exists or that a proposal remains relevant.

| Catalog | Scope | Original sources |
| --- | --- | --- |
| [Primary bugs](bugs.md) | 45 numbered reports, triage status, and links to related GitHub records | [`BUGS.txt`](../BUGS.txt) |
| [Secondary bugs](bugs-secondary.md) | 42 separate report files, including known issue and PR matches | [`bugs/`](../bugs/) |
| [Ideas and plans](ideas.md) | 44 TODO entries, 45 API wishes, 12 project ideas, and other historical files | [`todo.txt`](../todo.txt), [`api-wish-list.txt`](../api-wish-list.txt), [`projects.txt`](../projects.txt) |
| [Doxygen](doxygen.md) | Sources, build targets, publishing workflow, and documentation issue 119 | [`src/doc/`](../../src/doc/), [`docs/`](../) |

The [organization discussion #935](https://github.com/orgs/coin3d/discussions/935) gives the primary bug catalog a public entry point. Use namespaced identifiers such as `BUGS-219`, `SECONDARY-148`, `TODO-022`, `API-41`, and `PROJECT-11` when referring to historical entries; their numbers are not GitHub issue numbers. The original files remain the evidence for the old wording and dates. These catalogs record the triage and links to newer work.

## How to move an item into GitHub

1. Check whether an existing issue, pull request, or discussion already covers it. Record that link in the relevant catalog.
2. Reproduce the behavior on a current build, or document the missing environment and the evidence needed. A closed related issue alone does not prove the historical report is fixed.
3. Use an issue for a specific reproducible defect or agreed task. Use a discussion when the expected API, scope, or policy needs a decision.
4. Link the original entry and its namespaced ID from the GitHub record, then update the catalog with the outcome. Keep the historical source intact.

For a documentation report, link the historical entry to the source comment or `.dox` page, the generated API page, and the GitHub change that connects them. A successful build alone does not show that a type or page is indexed; check the generated output. See the [Doxygen map](doxygen.md) for the source and publishing paths.

The triage labels in [primary bugs](bugs.md) describe evidence available at the time of review, not current GitHub issue state. Recheck links and status before acting on them.
