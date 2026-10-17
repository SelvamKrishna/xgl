# git clone https://github.com/SelvamKrishna/pycraft pycraft

from dataclasses import dataclass
from pathlib import Path

import pycraft
import pycraft.config
import pycraft.package

assert pycraft.config.VERSION == 3

pycraft.init()

assert pycraft.verify_cmd("cmake", True)  # https://cmake.org/download/
assert pycraft.verify_cmd("make", True)  # https://www.msys2.org/#installation
assert pycraft.verify_cmd("glad", True)  # py -m pip install glad


@dataclass(frozen=True)
class Paths:
    working: Path = Path().cwd()  # noqa: RUF009
    vendor: Path = working / "vendor"
    build: Path = working / "build"
    source: Path = working / "source"


PATHS = Paths()


def _cmake_glfw(source_dir: Path, build_dir: Path) -> None:
    pycraft.run_cmd(
        [
            "cmake",
            "-S",
            str(source_dir),
            "-B",
            str(build_dir),
            "-G",
            "MinGW Makefiles",
            "-DGLFW_BUILD_EXAMPLES=OFF",
            "-DGLFW_BUILD_TESTS=OFF",
            "-DGLFW_BUILD_DOCS=OFF",
        ]
    )
    pycraft.run_cmd(f"make -C {build_dir}")


def glfw_package_setup() -> None:
    output_lib = PATHS.build / "libglfw3.a"
    if output_lib.exists():
        return

    temp_dir = PATHS.build / "temp" / "glfw"
    extract_dir = temp_dir / "build"

    pycraft.remove_path(temp_dir)

    pycraft.package.ArchivePackage(
        extract_dir,
        "https://github.com/glfw/glfw/releases/download/3.5.1/glfw-3.5.1.zip",
    ).install()

    pycraft.copy_path(extract_dir / "include" / "GLFW", PATHS.vendor / "GLFW")
    _cmake_glfw(extract_dir, temp_dir)
    pycraft.copy_path(temp_dir / "src" / "libglfw3.a", output_lib)


def glad_package_setup() -> None:
    output_c = PATHS.source / "glad.c"
    output_h = PATHS.vendor / "glad" / "glad.h"
    if output_c.exists() and output_h.exists():
        return

    temp_dir = PATHS.build / "temp" / "glad"

    pycraft.remove_path(temp_dir)

    pycraft.package.CustomPackage(
        temp_dir,
        f"py -m glad --profile=core --api=gl=4.6 --generator=c --out-path {temp_dir}",
    ).install()

    pycraft.copy_path(temp_dir / "include" / "glad", PATHS.vendor / "glad")
    pycraft.copy_path(temp_dir / "include" / "KHR", PATHS.vendor / "KHR")
    pycraft.copy_path(temp_dir / "src" / "glad.c", output_c)


if __name__ == "__main__":
    glfw_package_setup()
    glad_package_setup()
    pycraft.remove_path(PATHS.build / "temp")
