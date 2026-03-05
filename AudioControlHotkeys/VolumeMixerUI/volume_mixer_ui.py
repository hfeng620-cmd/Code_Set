import ctypes
import tkinter as tk
from tkinter import ttk
from ctypes import POINTER, cast

from comtypes import CLSCTX_ALL
from pycaw.pycaw import AudioUtilities, ISimpleAudioVolume

APP_TITLE = "Volume Mixer UI"
REFRESH_MS = 2000  # refresh session list every 2s

class SessionRow:
    def __init__(self, parent, session, remove_callback):
        self.session = session
        self.simple = session._ctl.QueryInterface(ISimpleAudioVolume)
        self.proc = session.Process
        self.name = self._display_name()

        self.frame = ttk.Frame(parent)
        self.label = ttk.Label(self.frame, text=self.name, width=32)
        self.var = tk.DoubleVar()
        self.slider = ttk.Scale(self.frame, from_=0, to=100, orient="horizontal", variable=self.var, command=self.on_slide)
        self.mute_var = tk.BooleanVar()
        self.mute = ttk.Checkbutton(self.frame, text="Mute", variable=self.mute_var, command=self.on_mute)
        self.remove_callback = remove_callback

        self.label.grid(row=0, column=0, sticky="w")
        self.slider.grid(row=0, column=1, sticky="ew", padx=(8, 8))
        self.mute.grid(row=0, column=2, sticky="e")
        self.frame.columnconfigure(1, weight=1)

        self.refresh()

    def _display_name(self):
        if self.proc is None:
            return "System Sounds"
        name = self.proc.name()
        return name

    def refresh(self):
        try:
            vol = self.simple.GetMasterVolume() * 100.0
            mute = self.simple.GetMute()
            self.var.set(vol)
            self.mute_var.set(bool(mute))
        except Exception:
            # Session may be gone
            self.remove_callback(self)

    def on_slide(self, _=None):
        try:
            v = max(0.0, min(100.0, float(self.var.get()))) / 100.0
            self.simple.SetMasterVolume(v, None)
        except Exception:
            self.remove_callback(self)

    def on_mute(self):
        try:
            self.simple.SetMute(bool(self.mute_var.get()), None)
        except Exception:
            self.remove_callback(self)

class VolumeMixerUI(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title(APP_TITLE)
        self.geometry("520x600")

        self.header = ttk.Frame(self)
        self.header.pack(fill="x", padx=12, pady=(12, 6))
        ttk.Label(self.header, text="Per-app volume (drag slider)", font=("Segoe UI", 11, "bold")).pack(side="left")
        ttk.Button(self.header, text="Refresh", command=self.refresh_sessions).pack(side="right")

        self.canvas = tk.Canvas(self, borderwidth=0, highlightthickness=0)
        self.scrollbar = ttk.Scrollbar(self, orient="vertical", command=self.canvas.yview)
        self.scrollable = ttk.Frame(self.canvas)

        self.scrollable.bind(
            "<Configure>",
            lambda e: self.canvas.configure(scrollregion=self.canvas.bbox("all"))
        )

        self.canvas_window = self.canvas.create_window((0, 0), window=self.scrollable, anchor="nw")
        self.canvas.configure(yscrollcommand=self.scrollbar.set)

        self.canvas.pack(side="left", fill="both", expand=True, padx=(12, 0), pady=(0, 12))
        self.scrollbar.pack(side="right", fill="y", padx=(0, 12), pady=(0, 12))

        self.rows = []
        self.after(100, self.refresh_sessions)
        self.after(REFRESH_MS, self.tick)

    def tick(self):
        self.refresh_sessions(update_only=True)
        self.after(REFRESH_MS, self.tick)

    def refresh_sessions(self, update_only=False):
        sessions = AudioUtilities.GetAllSessions()
        seen = set()

        if update_only:
            # Refresh existing rows and clean up missing ones
            for row in self.rows[:]:
                row.refresh()
            return

        # Full rebuild
        for row in self.rows:
            row.frame.destroy()
        self.rows = []

        for s in sessions:
            if s.Process is None and s.DisplayName is None:
                # keep system sounds
                pass
            sid = id(s)
            if sid in seen:
                continue
            seen.add(sid)
            row = SessionRow(self.scrollable, s, self.remove_row)
            row.frame.pack(fill="x", padx=4, pady=4)
            self.rows.append(row)

    def remove_row(self, row):
        try:
            row.frame.destroy()
        except Exception:
            pass
        if row in self.rows:
            self.rows.remove(row)

if __name__ == "__main__":
    ctypes.windll.shcore.SetProcessDpiAwareness(1)
    app = VolumeMixerUI()
    app.mainloop()
