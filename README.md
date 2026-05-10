# PSO PC V2 `psopol.dll`
<img width="1500" height="844" alt="Screenshot From 2026-05-08 14-56-07" src="https://github.com/user-attachments/assets/8e398e3c-8211-4aba-9352-74efad9b2740" />

Experimental runtime DLL work for **Phantasy Star Online PC V2**.

This repository is focused on `psopol.dll`: the project-specific runtime layer for widescreen, HUD/menu, culling, lighting/fog diagnostics, config, and logging.

## Current release target

**v0.1 alpha**

This is an early alpha baseline, not a stable release.

Current goals:

- Run PSO PC V2 at modern high resolutions.
- Present a 14:9 game viewport inside a 16:9 backbuffer.
- Keep black side bars outside the active 14:9 viewport.
- Keep HUD/menu positioning usable.
- Reduce widescreen-related object culling where possible.
- Preserve a playable baseline for further PC V2 DLL/proxy development.

## Disclaimer

This project includes LLM-assisted and LLM-generated source code that grew out of and still contains some remnants from earlier PSO PC V2 Dragon bug investigation work.

This code is experimental and should not be considered stable at this time.

## Files

### `psopol.c`

Source for `psopol.dll`.

This contains the project-specific PSO PC V2 runtime logic:

- 14:9 viewport policy
- widescreen projection handling
- HUD/menu correction
- PSO PC V2 memory patches
- culling-related runtime patches
- fog, lighting, and render-state diagnostics
- config handling
- logging

## 14:9 viewport strategy

- Full backbuffer: `3840x2160`
- Active 14:9 viewport: `3360x2160`
- Left/right black bars: `240px` each
- Projection correction factor: approximately `0.8571`

The 14:9 target was chosen because it currently behaves better than full 16:9 for PSO PC V2. Full 16:9 still exposes more severe culling, fade, and scene-composition problems.

## HUD and menu handling

The runtime includes HUD and screen-space correction logic for PSO PC V2's original 4:3 assumptions.

Current work includes:

- correcting transformed/screen-space draw paths
- keeping HUD/menu elements aligned inside the active viewport
- maintaining black bars outside the active game area
- handling conversation/fade/menu screens without exposing obvious viewport gaps

Some menu and HUD work remains experimental, but the current v0.1 baseline is playable.

## Culling work

Widescreen exposes several PSO PC V2 culling problems. This project treats culling as multiple internal paths rather than a single global bug.

The runtime patches several known or suspected culling-related values in PSO PC V2 memory:

- main frustum ratio
- static object threshold
- far-distance thresholds
- second culling cone values
- candidate effect/decal culling cone values
- per-bone or per-object cone globals

Known status:

- Distance-related popping has improved in some cases.
- Some culling still remains.
- Remaining culling is likely controlled by additional CPU-side visibility checks or object-class-specific paths.
- Future investigation may need to focus on object radius checks, portal/sector visibility, room render-list logic, or hardcoded screen-space assumptions.

## Lighting / fog / Caves rendering

Caves still has visible lighting or glow banding/lines in some rooms.

This is tracked separately from culling.

Likely investigation areas:

- fixed-function fog states
- alpha blend state
- z-write / depth interaction
- texture-stage behavior
- transparent glow-plane sorting
- legacy Direct3D8 fixed-function behavior under modern wrappers/drivers
- possible `SetLight` / `LightEnable` behavior

A previous render-state diagnostic was removed because per-call file I/O caused the game simulation to run in slow motion. Future diagnostics should use lightweight ring buffers, frame-limited logging, or config-gated sampling.

## Current known limitations

This is an alpha-quality build.

Known issues:

- Culling is improved but not fully fixed.
- Caves lighting/glow banding remains.
- Some lighting behavior may differ from original hardware/driver expectations.
- Full 16:9 is not currently supported; 14:9 is the current playable compromise.
- Some diagnostics and experimental scaffolding still need cleanup.

## Future plans

- `dinput` — high priority; Blue Burst-style keyboard controls.
- `dsound` — low priority.
