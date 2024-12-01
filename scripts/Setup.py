import sys

import os
import subprocess
import platform

from SetupSymlinks import SymlinksConfiguration as SymlinksRequirements
from SetupPython import PythonConfiguration as PythonRequirements

args = sys.argv

cla = False

if len(args) > 1:
    if args[1] == "cla":
        cla = True

# Make sure everything we need for the setup is installed
PythonRequirements.Validate(cla)
SymlinksRequirements.Validate()

from SetupPremake import PremakeConfiguration as PremakeRequirements
from SetupVulkan import VulkanConfiguration as VulkanRequirements
os.chdir('./../') # Change from devtools/scripts directory to root

premakeInstalled = PremakeRequirements.Validate(cla)
if not cla:
    VulkanRequirements.Validate()


print("\nUpdating submodules...")
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

if (premakeInstalled):
    if platform.system() == "Windows":
        print("\nRunning premake...")
        subprocess.call([os.path.abspath("./scripts/Win-GenProjects.bat"), "nopause", "cla" if cla else ""])

    print("\nSetup completed!")
else:
    print("Vertex requires Premake to generate project files.")

