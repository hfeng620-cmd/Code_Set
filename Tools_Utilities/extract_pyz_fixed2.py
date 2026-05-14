import sys
import os

# Set _MEIPASS to avoid AttributeError when importing PyInstaller loader
if not hasattr(sys, '_MEIPASS'):
    sys._MEIPASS = os.path.dirname(os.path.abspath(__file__))

# Add PyInstaller loader path
pyinstaller_path = r"D:\AAA_Learning_Resource\ScreenTracker\venv\Lib\site-packages\PyInstaller"
loader_path = os.path.join(pyinstaller_path, "loader")
if loader_path not in sys.path:
    sys.path.insert(0, loader_path)

try:
    from pyimod02_importers import ZlibArchiveReader
    HAS_PYINSTALLER = True
except ImportError as e:
    print(f"Failed to import ZlibArchiveReader: {e}")
    HAS_PYINSTALLER = False

def extract_pyz(pyz_file, output_dir):
    """Extract a PyInstaller PYZ archive."""
    if not HAS_PYINSTALLER:
        print("PyInstaller loader not available")
        return []
    
    try:
        ar = ZlibArchiveReader(pyz_file)
        print(f"Number of entries: {len(ar.toc)}")
        
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        
        extracted_files = []
        for name in ar.toc:
            try:
                data = ar.extract(name)
                # The extracted data is a .pyc file
                # Save it with .pyc extension
                file_path = os.path.join(output_dir, name.replace('.', os.sep) + '.pyc')
                os.makedirs(os.path.dirname(file_path), exist_ok=True)
                with open(file_path, 'wb') as f:
                    f.write(data)
                extracted_files.append((name, file_path))
                print(f"  Extracted: {name} -> {file_path}")
            except Exception as e:
                print(f"    Error extracting {name}: {e}")
        
        return extracted_files
    except Exception as e:
        print(f"Error reading PYZ archive: {e}")
        return []

def main():
    pyz_path = r'Code_Set\build\漂亮\PYZ-00.pyz'
    output_dir = r'Code_Set\build\漂亮\extracted_pyc'
    
    print(f"Extracting {pyz_path} to {output_dir}")
    extracted = extract_pyz(pyz_path, output_dir)
    
    # Look for Video_Player module
    video_player_pyc = None
    for name, path in extracted:
        if name == 'Video_Player' or 'video_player' in name.lower():
            video_player_pyc = path
            print(f"Found Video_Player module: {path}")
            break
    
    if video_player_pyc:
        print(f"\nVideo_Player .pyc file: {video_player_pyc}")
        
        # Try to decompile with uncompyle6
        try:
            import subprocess
            output_py = r'Code_Set\Video_Player_recovered.py'
            print(f"Decompiling to {output_py}...")
            result = subprocess.run(
                [sys.executable, '-m', 'uncompyle6', video_player_pyc, '-o', output_py],
                capture_output=True,
                text=True
            )
            if result.returncode == 0:
                print("Decompilation successful!")
                # Read and display first 100 lines
                with open(output_py, 'r', encoding='utf-8') as f:
                    lines = f.readlines()
                    print(f"\nFirst 100 lines of recovered code:")
                    for i, line in enumerate(lines[:100]):
                        print(f"{i+1}: {line}", end='')
            else:
                print(f"Decompilation failed: {result.stderr}")
        except Exception as e:
            print(f"Error during decompilation: {e}")
    else:
        print("\nVideo_Player module not found. Available modules:")
        for name, path in extracted:
            print(f"  {name}")

if __name__ == '__main__':
    main()
