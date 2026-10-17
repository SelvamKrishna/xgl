# git clone https://github.com/SelvamKrishna/pycraft pycraft

from pathlib import Path

import pycraft
import pycraft.cli
import pycraft.config

assert pycraft.config.VERSION == 3

BUILD_CFG = pycraft.cli.get_build_config()
pycraft.init()

WORKING_DIR = Path.cwd()
VENDOR_DIR = WORKING_DIR / "vendor"
BUILD_DIR = WORKING_DIR / "build"

FLAGS = pycraft.config.CompileFlags(
    version=23,
    includes=(
        WORKING_DIR,
        WORKING_DIR / "include",
        VENDOR_DIR,
        VENDOR_DIR / "glfw3" / "include",
        VENDOR_DIR / "glad" / "include",
        VENDOR_DIR / "zen-log" / "include",
        VENDOR_DIR / "zen-math" / "include",
    ),
    lib_dirs=(BUILD_DIR,),
    libraries=("glfw3", "gdi32", "opengl32"),
)

PROJ_CFG = pycraft.config.ProjectConfig(flags=FLAGS)

if __name__ == "__main__":
    pycraft.remove_path(BUILD_DIR / "temp")

    FLAGS.generate_compile_flags(Path("compile_flags.txt"))

    out_path = pycraft.Builder(PROJ_CFG, BUILD_CFG).build()

    if BUILD_CFG.should_run():
        pycraft.run_project(out_path)
