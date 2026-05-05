#!/usr/bin/env python3
import sys
import os
import struct
import zlib
import marshal

def read_pyz(pyz_file):
    """Read a PyInstaller PYZ archive and extract all .pyc files."""
    with open(pyz_file, 'rb') as f:
        # Check magic
        magic = f.read(4)
        if magic != b'PYZ\x00':
            print("Not a PYZ archive")
            return None
        
        # Read TOC offset and length
        toc_offset = struct.unpack('<I', f.read(4))[0]
        toc_length = struct.unpack('<I', f.read(4))[0]
        
        # Seek to TOC
        f.seek(toc_offset)
        toc_data = f.read(toc_length)
        
        # The TOC is a marshaled dictionary
        toc = marshal.loads(toc_data)
        
        # Extract each entry
        entries = []
        for name, (offset, length) in toc.items():
            f.seek(offset)
            compressed_data = f.read(length)
            # Decompress if needed (PYZ entries are usually compressed)
            try:
                data = zlib.decompress(compressed_data)
            except:
                data = compressed_data  # maybe not compressed
            
            entries.append((name, data))
            print(f"Extracted: {name} ({len(data)} bytes)")
        
        return entries

def find_video_player(entries):
    """Find Video_Player entry."""
    for name, data in entries:
        if name == 'Video_Player':
            return name, data
        if 'video' in name.lower():
            return name, data
    return None, None

def save_pyc(name, data, output_dir='extracted'):
    """Save .pyc data to file."""
    os.makedirs(output_dir, exist_ok=True)
    filename = f"{name}.pyc"
    path = os.path.join(output_dir, filename)
    with open(path, 'wb') as f:
        f.write(data)
    print(f"Saved to {path}")
    return path

def decompile_pyc(pyc_path, output_py):
    """Decompile .pyc to .py using uncompyle6."""
    import subprocess
    try:
        result = subprocess.run(
            [sys.executable, '-m', 'uncompyle6', pyc_path, '-o', output_py],
            capture_output=True,
            text=True,
            timeout=30
        )
        if result.returncode == 0:
            print(f"Decompiled to {output_py}")
            return True
        else:
            print(f"Decompilation failed: {result.stderr}")
            return False
    except Exception as e:
        print(f"Error during decompilation: {e}")
        return False

def main():
    pyz_path = r'Code_Set\build\漂亮\PYZ-00.pyz'
    if not os.path.exists(pyz_path):
        print(f"PYZ file not found: {pyz_path}")
        return
    
    print(f"Reading PYZ archive: {pyz_path}")
    entries = read_pyz(pyz_path)
    if not entries:
        print("No entries found or error reading PYZ")
        return
    
    print(f"\nTotal entries: {len(entries)}")
    
    # Find Video_Player
    name, data = find_video_player(entries)
    if not data:
        print("\nVideo_Player not found. Available entries:")
        for n, d in entries:
            print(f"  {n}")
        return
    
    print(f"\nFound Video_Player module: {name}")
    
    # Save .pyc
    pyc_path = save_pyc(name, data, 'Code_Set/extracted')
    
    # Decompile
    output_py = r'Code_Set\Video_Player_recovered.py'
    if decompile_pyc(pyc_path, output_py):
        # Read and show first 200 lines
        try:
            with open(output_py, 'r', encoding='utf-8') as f:
                lines = f.readlines()
                print(f"\nFirst 200 lines of recovered code (total {len(lines)} lines):")
                for i, line in enumerate(lines[:200]):
                    print(f"{i+1:4}: {line}", end='')
        except Exception as e:
            print(f"Error reading recovered file: {e}")
    else:
        print("Decompilation failed. Trying alternative method...")
        # Try to use the .pyc as is
        with open(output_py, 'wb') as f:
            f.write(data)
        print(f"Saved raw .pyc data to {output_py}")

if __name__ == '__main__':
    main()
