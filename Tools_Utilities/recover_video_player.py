import sys
import os
import tempfile
import shutil
import subprocess

def extract_pyz(pyz_file, output_dir):
    """Extract a PyInstaller PYZ archive and return the TOC."""
    try:
        from PyInstaller.loader.pyimod02_importers import ZlibArchiveReader
        ar = ZlibArchiveReader(pyz_file)
        print(f"Number of entries: {len(ar.toc)}")
        
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        
        extracted_files = []
        for name in ar.toc:
            # Only extract Python modules (ending with .pyc or without extension?)
            # The name might be like 'Video_Player' (without extension) but it's a .pyc inside.
            # We'll extract all.
            try:
                data = ar.extract(name)
                # The name might be a module name with dots. We'll convert to path.
                # For example, 'Video_Player' becomes 'Video_Player.pyc'
                # But note: the TOC entries are module names, not file names.
                # The ZlibArchiveReader extracts the module as a .pyc file.
                # We'll use the name as the file name with .pyc extension.
                if '.' in name:
                    # It's a submodule, we need to create directories.
                    parts = name.split('.')
                    dir_path = os.path.join(output_dir, *parts[:-1])
                    os.makedirs(dir_path, exist_ok=True)
                    file_path = os.path.join(dir_path, parts[-1] + '.pyc')
                else:
                    file_path = os.path.join(output_dir, name + '.pyc')
                
                with open(file_path, 'wb') as f:
                    f.write(data)
                extracted_files.append((name, file_path))
                print(f"  Extracted: {name} -> {file_path}")
            except Exception as e:
                print(f"    Error extracting {name}: {e}")
        
        return extracted_files
    except ImportError as e:
        print(f"Error importing PyInstaller: {e}")
        return []

def decompile_pyc(pyc_file, output_py):
    """Decompile a .pyc file to .py using uncompyle6 or decompyle3."""
    # Try uncompyle6 first, then decompyle3
    for tool in ['uncompyle6', 'decompyle3']:
        try:
            subprocess.run([sys.executable, '-m', tool, pyc_file, '-o', output_py], 
                          check=True, capture_output=True)
            print(f"Decompiled using {tool}: {pyc_file} -> {output_py}")
            return True
        except subprocess.CalledProcessError as e:
            print(f"{tool} failed: {e}")
        except Exception as e:
            print(f"{tool} not available: {e}")
    
    # If both fail, try using pycdc (if available) but we don't have it installed.
    return False

def main():
    # Paths
    pyz_path = r'Code_Set\build\漂亮\PYZ-00.pyz'
    output_dir = r'Code_Set\build\漂亮\extracted2'
    
    # Step 1: Extract PYZ
    print("Extracting PYZ archive...")
    extracted = extract_pyz(pyz_path, output_dir)
    
    # Step 2: Find Video_Player module
    video_player_pyc = None
    for name, path in extracted:
        if name == 'Video_Player':
            video_player_pyc = path
            break
    
    if not video_player_pyc:
        # Maybe the module name is different? Try to find any file with 'video' in the name (case-insensitive)
        for name, path in extracted:
            if 'video' in name.lower():
                video_player_pyc = path
                print(f"Found alternative: {name}")
                break
    
    if video_player_pyc and os.path.exists(video_player_pyc):
        print(f"Found Video_Player module: {video_player_pyc}")
        # Step 3: Decompile
        output_py = r'Code_Set\Video_Player_recovered.py'
        if decompile_pyc(video_player_pyc, output_py):
            print(f"Successfully recovered to {output_py}")
        else:
            print("Failed to decompile. Trying to read the .pyc as raw bytes and write as .py?")
            # If decompilation fails, at least copy the .pyc to .py (but it's bytecode)
            with open(video_player_pyc, 'rb') as f:
                data = f.read()
            with open(output_py, 'wb') as f:
                f.write(data)
            print(f"Copied .pyc to {output_py} (not decompiled)")
    else:
        print("Video_Player module not found in the PYZ archive.")
        # List all extracted files
        print("Available modules:")
        for name, _ in extracted:
            print(f"  {name}")

if __name__ == '__main__':
    main()
