import sys
import os
import struct
import zlib
import marshal
import tempfile
import shutil

def extract_pyz(pyz_file, output_dir):
    """Extract a PyInstaller PYZ archive."""
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    with open(pyz_file, 'rb') as f:
        # Read the header
        magic = f.read(4)
        if magic != b'PYZ\x00':
            print("Not a PYZ archive")
            return
        
        # Skip the TOC offset (4 bytes) and read the TOC
        f.seek(8, 1)  # skip the TOC offset and the TOC length (both 4 bytes)
        toc_data = f.read()
    
    # The TOC is a marshaled list of (name, offset, length) tuples
    # But note: the PYZ archive format may have changed. This is a simplified extractor.
    # We'll use a more robust method: use PyInstaller's archive reader if available.
    try:
        from PyInstaller.loader.pyimod02_importers import ZlibArchiveReader
        ar = ZlibArchiveReader(pyz_file)
        print(f"Number of entries: {len(ar.toc)}")
        for name in ar.toc:
            print(f"  {name}")
            try:
                data = ar.extract(name)
                # Write the data to a file
                out_path = os.path.join(output_dir, name.replace('.', os.sep))
                # If it's a .pyc file, we can try to decompile it later
                os.makedirs(os.path.dirname(out_path), exist_ok=True)
                with open(out_path, 'wb') as out:
                    out.write(data)
            except Exception as e:
                print(f"    Error extracting {name}: {e}")
    except ImportError:
        print("PyInstaller not available, using fallback extraction")
        # Fallback: try to read the TOC and extract each entry
        # This is more complex and may not work for all versions.
        pass

if __name__ == '__main__':
    pyz_path = r'Code_Set\build\漂亮\PYZ-00.pyz'
    output_dir = r'Code_Set\build\漂亮\extracted'
    extract_pyz(pyz_path, output_dir)
    print(f"Extracted to {output_dir}")
