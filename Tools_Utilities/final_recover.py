#!/usr/bin/env python3
"""
Final recovery script for Video_Player.py from PyInstaller executable.
This script combines multiple approaches to recover the complete source code.
"""

import sys
import os
import struct
import zlib
import marshal
import subprocess
import tempfile

def read_pyz_direct(pyz_file):
    """Read PYZ archive directly using low-level parsing."""
    print(f"Reading PYZ file: {pyz_file}")
    
    with open(pyz_file, 'rb') as f:
        # Read and verify magic
        magic = f.read(4)
        if magic != b'PYZ\x00':
            print(f"Invalid magic: {magic}")
            return None
        
        # Read TOC offset and length
        try:
            toc_offset = struct.unpack('<I', f.read(4))[0]
            toc_length = struct.unpack('<I', f.read(4))[0]
        except struct.error as e:
            print(f"Error reading TOC header: {e}")
            return None
        
        print(f"TOC offset: {toc_offset}, TOC length: {toc_length}")
        
        # Seek to TOC
        f.seek(toc_offset)
        toc_data = f.read(toc_length)
        
        if len(toc_data) != toc_length:
            print(f"Failed to read full TOC: got {len(toc_data)} bytes, expected {toc_length}")
            return None
        
        # Try to unmarshal TOC
        try:
            toc = marshal.loads(toc_data)
            print(f"Successfully loaded TOC with {len(toc)} entries")
        except Exception as e:
            print(f"Failed to unmarshal TOC: {e}")
            # Try to find marshal data manually
            return None
        
        # Extract each entry
        entries = []
        for name, (offset, length) in toc.items():
            f.seek(offset)
            compressed_data = f.read(length)
            
            # Try to decompress
            try:
                data = zlib.decompress(compressed_data)
                is_compressed = True
            except:
                data = compressed_data
                is_compressed = False
            
            entries.append({
                'name': name,
                'data': data,
                'compressed': is_compressed,
                'offset': offset,
                'length': length
            })
        
        return entries

def find_main_module(entries):
    """Find the main Video_Player module."""
    # Look for exact match first
    for entry in entries:
        if entry['name'] == 'Video_Player':
            return entry
    
    # Look for case-insensitive match
    for entry in entries:
        if 'video' in entry['name'].lower() and 'player' in entry['name'].lower():
            return entry
    
    # Look for any module that might be the main one
    for entry in entries:
        if not entry['name'].startswith('PyQt') and not entry['name'].startswith('_'):
            return entry
    
    return None

def save_and_decompile(entry, output_dir):
    """Save .pyc file and decompile it."""
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    # Save as .pyc
    pyc_path = os.path.join(output_dir, f"{entry['name']}.pyc")
    with open(pyc_path, 'wb') as f:
        f.write(entry['data'])
    
    print(f"Saved .pyc file: {pyc_path} ({len(entry['data'])} bytes)")
    
    # Try to decompile with uncompyle6
    py_path = os.path.join(output_dir, f"{entry['name']}_recovered.py")
    
    try:
        print(f"Attempting to decompile with uncompyle6...")
        result = subprocess.run(
            [sys.executable, '-m', 'uncompyle6', pyc_path, '-o', py_path],
            capture_output=True,
            text=True,
            timeout=60
        )
        
        if result.returncode == 0:
            print(f"Successfully decompiled to: {py_path}")
            return py_path
        else:
            print(f"Decompilation failed: {result.stderr[:500]}")
            return None
    except Exception as e:
        print(f"Error during decompilation: {e}")
        return None

def combine_existing_code(recovered_path):
    """Combine recovered code with existing partial code."""
    # Read existing partial files
    existing_files = [
        'Code_Set/Video_Player.py',
        'Code_Set/Video_Player_annotated.py', 
        'Code_Set/Video_Player_backup.py'
    ]
    
    existing_content = []
    for file_path in existing_files:
        if os.path.exists(file_path):
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
                existing_content.append(f"=== {file_path} ===\n{content}\n")
    
    # Read recovered code if it exists
    recovered_content = ""
    if recovered_path and os.path.exists(recovered_path):
        with open(recovered_path, 'r', encoding='utf-8') as f:
            recovered_content = f.read()
    
    # Combine all into a final file
    final_path = 'Code_Set/Video_Player_complete.py'
    
    with open(final_path, 'w', encoding='utf-8') as f:
        f.write("# ============================================================================\n")
        f.write("# RECOVERED VIDEO PLAYER - COMPLETE VERSION\n")
        f.write("# Combined from multiple sources\n")
        f.write("# ============================================================================\n\n")
        
        if recovered_content:
            f.write("# --- Decompiled from executable ---\n")
            f.write(recovered_content)
            f.write("\n\n")
        
        if existing_content:
            f.write("# --- Existing partial code ---\n")
            for content in existing_content:
                f.write(content)
                f.write("\n")
    
    print(f"Combined code saved to: {final_path}")
    return final_path

def main():
    # Paths
    pyz_path = r'Code_Set\build\漂亮\PYZ-00.pyz'
    output_dir = r'Code_Set\recovered'
    
    if not os.path.exists(pyz_path):
        print(f"PYZ file not found: {pyz_path}")
        print("Trying alternative approach...")
        
        # Check if we can use the executable directly
        exe_path = r'Code_Set\dist\漂亮\漂亮.exe'
        if os.path.exists(exe_path):
            print(f"Found executable: {exe_path}")
            print("Note: Direct executable extraction is more complex.")
            print("We'll work with the PYZ file from build directory.")
        return
    
    # Step 1: Extract from PYZ
    entries = read_pyz_direct(pyz_path)
    if not entries:
        print("Failed to extract from PYZ file")
        return
    
    print(f"\nExtracted {len(entries)} modules")
    for i, entry in enumerate(entries[:10]):  # Show first 10
        print(f"  {i+1}. {entry['name']} ({len(entry['data'])} bytes)")
    
    if len(entries) > 10:
        print(f"  ... and {len(entries) - 10} more")
    
    # Step 2: Find main module
    main_entry = find_main_module(entries)
    if not main_entry:
        print("\nCould not find main Video_Player module")
        print("Available modules:")
        for entry in entries:
            print(f"  {entry['name']}")
        return
    
    print(f"\nFound main module: {main_entry['name']}")
    
    # Step 3: Save and decompile
    recovered_py = save_and_decompile(main_entry, output_dir)
    
    # Step 4: Combine with existing code
    if recovered_py:
        final_path = combine_existing_code(recovered_py)
        
        # Show first 50 lines of the final file
        try:
            with open(final_path, 'r', encoding='utf-8') as f:
                lines = f.readlines()
                print(f"\nFirst 50 lines of recovered code (total {len(lines)} lines):")
                for i, line in enumerate(lines[:50]):
                    print(f"{i+1:4}: {line}", end='')
                
                # Check for AudioVideoPlayer class
                content = ''.join(lines)
                if 'class AudioVideoPlayer' in content:
                    print("\n✓ Found AudioVideoPlayer class in recovered code!")
                else:
                    print("\n✗ AudioVideoPlayer class not found in recovered code")
        except Exception as e:
            print(f"Error reading final file: {e}")
    else:
        print("\nFailed to decompile main module")
        print("Creating combined file from existing partial code only...")
        final_path = combine_existing_code(None)
    
    print(f"\n{'='*60}")
    print("RECOVERY SUMMARY:")
    print(f"{'='*60}")
    print(f"1. PYZ file: {pyz_path}")
    print(f"2. Main module: {main_entry['name'] if main_entry else 'Not found'}")
    print(f"3. Recovered .py: {recovered_py if recovered_py else 'Failed'}")
    print(f"4. Final combined file: {final_path}")
    print(f"{'='*60}")

if __name__ == '__main__':
    main()
