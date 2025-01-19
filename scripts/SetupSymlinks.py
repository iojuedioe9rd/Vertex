import os
import shutil

class SymlinksConfiguration:
    Symlinks = {"../Engine/vendor/mono/lib/Release": "../Engine/vendor/mono/lib/Dist",
                "../Engine/vendor/ffmpeg-cpp/lib/x64/Release": "../Engine/vendor/ffmpeg-cpp/lib/x64/Dist"}

    @classmethod
    def CheckLinks(cls):
        for target, key in cls.Symlinks.items():
            if not os.path.exists(key) or not os.path.exists(target) or not os.path.samefile(key, target):
                print(f"Link '{key}' is not pointing to '{target}' or one of them does not exist.")
                return False
        return True

    @classmethod
    def Validate(cls):
        if not cls.CheckLinks():
            print("Links are not configured.")
            for target, key in cls.Symlinks.items():
                try:
                    # Remove any existing file or directory at `key`
                    if os.path.exists(key):
                        if os.path.isfile(key):
                            os.remove(key)
                        else:
                            shutil.rmtree(key)
                    
                    # Copy the `target` content to `key`
                    print(f"Copying '{target}' to '{key}'")
                    if os.path.isdir(target):
                        shutil.copytree(target, key)
                    else:
                        shutil.copy2(target, key)
                    print(f"Link '{key}' created as a copy successfully.")
                except Exception as e:
                    print(f"Failed to create link '{key}' due to: {e}")
                    return False
        print("Links are configured.")
        return True
import os
import shutil

class SymlinksConfiguration:
    Symlinks = {"../Engine/vendor/mono/lib/Release": "../Engine/vendor/mono/lib/Dist"}

    @classmethod
    def CheckLinks(cls):
        for target, key in cls.Symlinks.items():
            if not os.path.exists(key) or not os.path.exists(target) or not os.path.samefile(key, target):
                print(f"Link '{key}' is not pointing to '{target}' or one of them does not exist.")
                return False
        return True

    @classmethod
    def Validate(cls):
        if not cls.CheckLinks():
            print("Links are not configured.")
            for target, key in cls.Symlinks.items():
                try:
                    # Remove any existing file or directory at `key`
                    if os.path.exists(key):
                        if os.path.isfile(key):
                            os.remove(key)
                        else:
                            shutil.rmtree(key)
                    
                    # Copy the `target` content to `key`
                    print(f"Copying '{target}' to '{key}'")
                    if os.path.isdir(target):
                        shutil.copytree(target, key)
                    else:
                        shutil.copy2(target, key)
                    print(f"Link '{key}' created as a copy successfully.")
                except Exception as e:
                    print(f"Failed to create link '{key}' due to: {e}")
                    return False
        print("Links are configured.")
        return True
