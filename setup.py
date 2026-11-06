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
assert pycraft.verify_cmd("glad", True)  # python -m pip install glad


@dataclass(frozen=True)
class Paths:
    working: Path = Path().cwd()  # noqa: RUF009
    vendor: Path = working / "vendor"
    build: Path = working / "build"
    source: Path = working / "source"


PATHS = Paths()


def glfw_package_setup() -> None:
    if (PATHS.build / "libglfw3.a").exists():
        return

    if not (PATHS.build / "glfw").exists():
        pycraft.run_cmd(
            [
                "cmake",
                "-S",
                str(PATHS.vendor / "glfw"),
                "-B",
                str(PATHS.build / "glfw"),
                "-G",
                "MinGW Makefiles",
                "-DGLFW_BUILD_EXAMPLES=OFF",
                "-DGLFW_BUILD_TESTS=OFF",
                "-DGLFW_BUILD_DOCS=OFF",
            ]
        )

    if not (PATHS.build / "glfw" / "src" / "libglfw3.a").exists():
        pycraft.run_cmd(f"make -C {PATHS.build / 'glfw'}")

    pycraft.copy_path(PATHS.build / 'glfw' / "src" / "libglfw3.a", PATHS.build / "libglfw3.a")

def glad_package_setup() -> None:
    output_c = PATHS.source / "glad.c"
    output_h = PATHS.vendor / "glad" / "glad.h"
    if output_c.exists() and output_h.exists():
        return

    temp_dir = PATHS.build / "temp" / "glad"

    pycraft.remove_path(temp_dir)

    pycraft.package.CustomPackage(
        temp_dir,
        f"python -m glad --profile=core --api=gl=4.6 --generator=c --out-path {temp_dir}",
    ).install()

    pycraft.copy_path(temp_dir / "include" / "glad", PATHS.vendor / "glad")
    pycraft.copy_path(temp_dir / "include" / "KHR", PATHS.vendor / "KHR")
    pycraft.copy_path(temp_dir / "src" / "glad.c", output_c)


if __name__ == "__main__":
    glfw_package_setup()
    glad_package_setup()
    pycraft.remove_path(PATHS.build / "temp")
