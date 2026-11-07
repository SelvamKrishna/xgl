from pathlib import Path

for f in [
"buffer.cpp",
"config.cpp",
"engine.cpp",
"render_state.cpp",
"shader.cpp",
"texture.cpp",
"vertex_array.cpp",
"window.cpp",
"_gl_object.cpp",
]: (Path("source") / f).touch()
