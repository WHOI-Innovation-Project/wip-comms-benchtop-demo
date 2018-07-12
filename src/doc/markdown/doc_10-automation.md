## Automation

Many routine tasks can be automated to ensure consistency, correctness, and reduce the need for human interventions.

### Continuous Integration (CI)

CI is the process of building and testing the code base, typically on every commit, to reduce errors as early as possible in the development cycle.

We are using CircleCI configured with Debian package builds that will produce a .deb file suitable for immediate deployment (e.g. on to the Raspberry Pi Compute Module computer).

Every commit is built for the standard laptop/desktop 64-bit x86 architecture (`amd64`) and cross-built for the ARM architecture (`armhf`).

Unit tests are run on the `amd64` build.

### Static Analysis (in progress)

Static analysis examines the code base for errors based on the code itself. Well written modern C++ benefits already from a significant amount of compiler-based static analysis (templates, static_assert, etc.).

TODO - set up https://scan.coverity.com/