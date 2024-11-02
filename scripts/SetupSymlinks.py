import os
import shutil

class SymlinksConfiguration:
    Symlinks = {"../Engine/vendor/mono/lib/Release": "../Engine/vendor/mono/lib/Dist"}

    @classmethod
    def CheckLinks(cls):
        for key, target in cls.Symlinks.items():
            if not os.path.exists(key) or not os.path.samefile(key, target):
                print(f"Link '{key}' is not pointing to '{target}'")
                return False
        return True

    @classmethod
    def Validate(cls):
        if not cls.CheckLinks():
            print("Links are not configured.")
            for key, target in cls.Symlinks.items():
                try:
                    if os.path.exists(key):
                        if os.path.isfile(key):
                            os.remove(key)
                        else:
                            shutil.rmtree(key)
                    print(f"Copying '{target}' to '{key}'")
                    shutil.copytree(target, key) if os.path.isdir(target) else shutil.copy2(target, key)
                    print(f"Link '{key}' created as a copy successfully.")
                except Exception as e:
                    print(f"Failed to create link '{key}' due to: {e}")
                    return False
        print("Links are configured.")
        return True
