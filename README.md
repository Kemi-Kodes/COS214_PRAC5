# CampusGuard

COS 214 Practical 5 — a C++11 emergency-response console for a university campus.
An incident is reported, response teams are dispatched, buildings are locked down,
alerts go out, and the incident is resolved — all driven by six collaborating
design patterns.

## Patterns used

| Pattern   | Role in CampusGuard                                                        | Key classes                                         |
| --------- | -------------------------------------------------------------------------- | --------------------------------------------------- |
| Command   | Operator actions (dispatch, lock, alert, cancel) run through one invoker   | `Command`, `OperatorConsole`                        |
| Mediator  | Response teams coordinate through one object instead of calling each other | `ResponseCoordinator`, `CampusResponseCoordinator`  |
| Adapter   | Wraps a legacy siren system behind our modern alert interface              | `AlertService`, `SirenAdapter`, `LegacySirenSystem` |
| Facade    | One call (`startEvacuation`) hides locking, dispatch and alerting          | `CampusGuardSystem`                                 |
| State     | An incident moves Reported → Dispatched → Contained → Resolved             | `Incident`, `State` and its subclasses              |
| Composite | Lock one room or a whole building with the same call                       | `CampusArea`, `Building`, `Room`                    |

## Folder structure

```
.
├── main.cpp              # both the scripted demo and the interactive console (see below)
├── Makefile
├── Dockerfile
├── docker-compose.yml
├── Adapter/              # AlertService, SirenAdapter, LegacySirenSystem
├── Command/              # Command, OperatorConsole
├── Compisite/            # CampusArea, Building, Room
├── Facade/               # CampusGuardSystem
├── Mediator/             # ResponseCoordinator, teams, CommsService
├── State/                # Incident, State
└── tests/                # standalone test/reference files, never built into the app
```

## Building and running

### With Docker (this is what we use for the demo)

```bash
docker compose up --build
```

This builds the image (g++, Valgrind and GDB included) and runs the two required
scripted scenarios:

1. **Fire in the Library** — one continuous flow where all six patterns
   collaborate: Facade locks the building and dispatches Security (Composite +
   Command), the incident moves through its states (State), Security escalates
   to the Mediator (Mediator), every alert goes out through the legacy siren
   (Adapter), and the Facade stands the area back down at the end.
2. **Medical emergency, Security unavailable** — Security is already deployed
   elsewhere, so when Medical calls in a casualty the Mediator has to route
   around the conflict; the operator dispatches Medical by Command, the area
   gets restricted (Composite), and it turns out to be a false alarm that gets
   cancelled (State + Command).

To run the interactive console instead, inside Docker:

```bash
docker compose run --rm campusguard ./campusguard --interactive
```

To run Valgrind or GDB inside the same environment used for the demo:

```bash
docker compose run --rm campusguard make valgrind
docker compose run --rm campusguard gdb ./campusguard
```

### Locally (WSL / Linux), without Docker

```bash
make            # builds ./campusguard
make run                # the two scripted scenarios
make run-interactive    # the menu-driven operator console
make valgrind           # leak check on the scripted run
make clean              # removes build/ and the binaries
```

## Two ways to run the same program

`main.cpp` builds one executable with two modes, picked at startup:

```bash
./campusguard                  # scripted: two fixed end-to-end stories (default)
./campusguard --interactive    # a menu you drive yourself, same underlying objects
```

Both modes build the exact same Command/Mediator/Adapter/Facade/State/Composite
objects — the interactive console is just a different way of triggering them, it
does not duplicate any logic.

## Engineering notes

- Every polymorphic base class has a virtual destructor; ownership is documented
  in the PDF report's "design and ownership rationale" section.
- `make valgrind` shows 0 leaks on the scripted run.
- A real bug (a double-free from deleting a `Room` without removing it from its
  `Building` first) was found and fixed during development — see the GDB
  investigation in the PDF report.

## Team

| Person   | Patterns          | Main folders            |
| -------- | ----------------- | ----------------------- |
| Person A | Command, State    | `Command/`, `State/`    |
| Person B | Mediator, Adapter | `Mediator/`, `Adapter/` |
| Person C | Composite, Facade | `Compisite/`, `Facade/` |

Replace the placeholder names above with your actual names before submitting.
