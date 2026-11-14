#!/usr/bin/env python3
"""
dbcEditor.py - A visual desktop editor for Vector DBC CAN database files.

Allows users to create, view, search, add, edit, and remove CAN messages
and signals with full attribute, cycle-time, and comment preservation.

Usage:
    python tools/dbcEditor.py [path_to_file.dbc]
"""

import os
import sys
import re
import math
import argparse
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
if str(PROJECT_ROOT) not in sys.path:
    sys.path.insert(0, str(PROJECT_ROOT))

try:
    import cantools
    from cantools.database.can import Database, Message, Signal
    from cantools.database.conversion import BaseConversion
except ImportError:
    sys.exit("error: 'cantools' is required. Please install it using: pip install cantools")

try:
    import tkinter as tk
    from tkinter import ttk, messagebox, filedialog
    _Toplevel = tk.Toplevel
    _Tk = tk.Tk
except ImportError:
    tk = None
    ttk = None
    messagebox = None
    filedialog = None
    _Toplevel = object
    _Tk = object


# ─────────────────────────────────────────────────────────────────────────────
# Core Data Model
# ─────────────────────────────────────────────────────────────────────────────

class DbcModel:
    """Manages the in-memory cantools CAN database and modifications."""

    def __init__(self, filepath=None):
        self.filepath = Path(filepath) if filepath else None
        self.db = None
        self.is_dirty = False
        if self.filepath and self.filepath.is_file():
            self.load(self.filepath)
        else:
            self.new_database()

    def new_database(self):
        self.db = Database(strict=False)
        self.filepath = None
        self.is_dirty = False

    def load(self, filepath):
        path = Path(filepath)
        # Load with strict=False to support real-world multiplexed/UDS DBCs
        self.db = cantools.database.load_file(str(path), strict=False)
        self.filepath = path
        self.is_dirty = False

    def save(self, filepath=None, auto_generate_header=False, header_path=None):
        target = Path(filepath) if filepath else self.filepath
        if not target:
            raise ValueError("No destination file path provided.")

        dbc_content = self.db.as_dbc_string()
        target.write_text(dbc_content, encoding="latin-1")
        self.filepath = target
        self.is_dirty = False

        generated_header = None
        if auto_generate_header and self.db and self.db.messages:
            generated_header = self.generate_header(output_path=header_path)
        return generated_header

    def generate_header(self, output_path=None, master_array_name="artemis_messages"):
        """Compile in-memory CAN database to a C++ header (artemis_canid.hpp)."""
        try:
            from tools.toHeader import generate_header_from_db
        except ImportError:
            from toHeader import generate_header_from_db
        target = Path(output_path) if output_path else (PROJECT_ROOT / "Inc" / "artemis_canid.hpp")
        source_name = self.filepath.name if self.filepath else "dbcEditor"
        content = generate_header_from_db(
            self.db,
            master_array_name=master_array_name,
            source_file=source_name
        )
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(content, encoding="utf-8")
        return target

    @property
    def messages(self):
        return sorted(self.db.messages, key=lambda m: m.frame_id) if self.db else []

    @property
    def nodes(self):
        return sorted([node.name for node in self.db.nodes]) if self.db else []

    def get_cycle_time(self, message):
        """Retrieve GenMsgCycleTime attribute if present."""
        if not message:
            return 0
        cycle = 0
        if hasattr(message, "cycle_time") and message.cycle_time is not None:
            cycle = message.cycle_time
        elif hasattr(message, "attributes") and "GenMsgCycleTime" in message.attributes:
            cycle = message.attributes.get("GenMsgCycleTime", 0)
        return int(cycle or 0)

    def set_cycle_time(self, message, cycle_ms):
        """Update or insert GenMsgCycleTime attribute."""
        if hasattr(message, "cycle_time"):
            message.cycle_time = int(cycle_ms) if cycle_ms > 0 else None
        if not hasattr(message, "attributes") or message.attributes is None:
            message.attributes = {}
        if cycle_ms > 0:
            message.attributes["GenMsgCycleTime"] = int(cycle_ms)
        elif "GenMsgCycleTime" in message.attributes:
            del message.attributes["GenMsgCycleTime"]

    def add_message(self, frame_id, name, length, senders, comment="", cycle_time=0, is_extended=False):
        # Check duplicate ID
        for m in self.db.messages:
            if m.frame_id == frame_id:
                raise ValueError(f"Message ID 0x{frame_id:X} already exists ({m.name}).")
            if m.name.lower() == name.lower():
                raise ValueError(f"Message name '{name}' already exists.")

        msg = Message(
            frame_id=frame_id,
            name=name,
            length=length,
            signals=[],
            senders=senders if isinstance(senders, list) else [senders],
            comment=comment or None,
            is_extended_frame=is_extended or (frame_id > 0x7FF)
        )
        self.set_cycle_time(msg, cycle_time)
        self.db.messages.append(msg)
        self.is_dirty = True
        return msg

    def edit_message(self, orig_msg, frame_id, name, length, senders, comment="", cycle_time=0, is_extended=False):
        # Validate ID collisions
        for m in self.db.messages:
            if m != orig_msg:
                if m.frame_id == frame_id:
                    raise ValueError(f"Message ID 0x{frame_id:X} is already used by '{m.name}'.")
                if m.name.lower() == name.lower():
                    raise ValueError(f"Message name '{name}' already exists.")

        backup = (orig_msg.frame_id, orig_msg.name, orig_msg.length, orig_msg._senders,
                  orig_msg.comment, orig_msg.is_extended_frame, self.get_cycle_time(orig_msg))

        orig_msg.frame_id = frame_id
        orig_msg.name = name
        orig_msg.length = length
        orig_msg._senders = senders if isinstance(senders, list) else ([senders] if senders else [])
        orig_msg.comment = comment or None
        orig_msg.is_extended_frame = is_extended or (frame_id > 0x7FF)
        self.set_cycle_time(orig_msg, cycle_time)

        try:
            if hasattr(orig_msg, "refresh"):
                orig_msg.refresh()
        except Exception as err:
            (orig_msg.frame_id, orig_msg.name, orig_msg.length, orig_msg._senders,
             orig_msg.comment, orig_msg.is_extended_frame, old_cycle) = backup
            self.set_cycle_time(orig_msg, old_cycle)
            raise ValueError(f"Cannot resize message '{name}' to {length} bytes: {err}")

        self.is_dirty = True
        return orig_msg

    def remove_message(self, message):
        if message in self.db.messages:
            self.db.messages.remove(message)
            self.is_dirty = True

    def add_signal(self, message, name, start, length, byte_order="little_endian", is_signed=False,
                   scale=1.0, offset=0.0, unit="", minimum=None, maximum=None, comment=None):
        for s in message.signals:
            if s.name.lower() == name.lower():
                raise ValueError(f"Signal '{name}' already exists in message '{message.name}'.")

        sig = Signal(
            name=name,
            start=start,
            length=length,
            byte_order=byte_order,
            is_signed=is_signed,
            conversion=BaseConversion.factory(scale=scale, offset=offset),
            unit=unit or "",
            minimum=minimum,
            maximum=maximum,
            comment=comment or None
        )
        message.signals.append(sig)
        try:
            if hasattr(message, "refresh"):
                message.refresh()
        except Exception as err:
            message.signals.remove(sig)
            raise ValueError(f"Signal '{name}' does not fit in message '{message.name}' (DLC={message.length} bytes): {err}")
        self.is_dirty = True
        return sig

    def edit_signal(self, message, orig_sig, name, start, length, byte_order="little_endian", is_signed=False,
                    scale=1.0, offset=0.0, unit="", minimum=None, maximum=None, comment=None):
        for s in message.signals:
            if s != orig_sig and s.name.lower() == name.lower():
                raise ValueError(f"Signal '{name}' already exists in message '{message.name}'.")

        backup = (orig_sig.name, orig_sig.start, orig_sig.length, orig_sig.byte_order,
                  orig_sig.is_signed, orig_sig.conversion, orig_sig.unit, orig_sig.minimum,
                  orig_sig.maximum, orig_sig.comment)

        orig_sig.name = name
        orig_sig.start = start
        orig_sig.length = length
        orig_sig.byte_order = byte_order
        orig_sig.is_signed = is_signed
        orig_sig.conversion = BaseConversion.factory(scale=scale, offset=offset)
        orig_sig.unit = unit or ""
        orig_sig.minimum = minimum
        orig_sig.maximum = maximum
        orig_sig.comment = comment or None
        try:
            if hasattr(message, "refresh"):
                message.refresh()
        except Exception as err:
            (orig_sig.name, orig_sig.start, orig_sig.length, orig_sig.byte_order,
             orig_sig.is_signed, orig_sig.conversion, orig_sig.unit, orig_sig.minimum,
             orig_sig.maximum, orig_sig.comment) = backup
            raise ValueError(f"Signal '{name}' does not fit in message '{message.name}' (DLC={message.length} bytes): {err}")
        self.is_dirty = True
        return orig_sig

    def remove_signal(self, message, signal):
        if signal in message.signals:
            message.signals.remove(signal)
            if hasattr(message, "refresh"):
                message.refresh()
            self.is_dirty = True


# ─────────────────────────────────────────────────────────────────────────────
# Dialogs
# ─────────────────────────────────────────────────────────────────────────────

class MessageDialog(_Toplevel):
    """Modal dialog to create or edit a CAN Message."""

    def __init__(self, parent, model, message=None):
        super().__init__(parent)
        self.model = model
        self.message = message
        self.result = None

        self.title("Edit Message" if message else "Add CAN Message")
        self.geometry("480x420")
        self.resizable(False, False)
        self.transient(parent)
        self.grab_set()

        self._build_ui()
        if self.message:
            self._populate()

    def _build_ui(self):
        pad = {"padx": 10, "pady": 6}
        form = ttk.Frame(self, padding=15)
        form.pack(fill=tk.BOTH, expand=True)

        # Message Name
        ttk.Label(form, text="Message Name:").grid(row=0, column=0, sticky=tk.W, **pad)
        self.ent_name = ttk.Entry(form, width=28)
        self.ent_name.grid(row=0, column=1, sticky=tk.W, **pad)

        # CAN ID (Hex / Dec)
        ttk.Label(form, text="CAN ID (hex or dec):").grid(row=1, column=0, sticky=tk.W, **pad)
        self.ent_id = ttk.Entry(form, width=28)
        self.ent_id.grid(row=1, column=1, sticky=tk.W, **pad)
        ttk.Label(form, text="e.g. 0x200 or 512", font=("TkDefaultFont", 8, "italic")).grid(row=1, column=2, sticky=tk.W)

        # DLC / Length
        ttk.Label(form, text="Length / DLC (bytes):").grid(row=2, column=0, sticky=tk.W, **pad)
        self.spn_length = ttk.Spinbox(form, from_=0, to=64, width=6)
        self.spn_length.set(8)
        self.spn_length.grid(row=2, column=1, sticky=tk.W, **pad)

        # Sender Node
        ttk.Label(form, text="Sender Node:").grid(row=3, column=0, sticky=tk.W, **pad)
        nodes = self.model.nodes or ["Vector__XXX"]
        self.cb_sender = ttk.Combobox(form, values=nodes, width=25)
        if nodes:
            self.cb_sender.set(nodes[0])
        self.cb_sender.grid(row=3, column=1, sticky=tk.W, **pad)

        # Cycle Time (ms)
        ttk.Label(form, text="Cycle Time (ms):").grid(row=4, column=0, sticky=tk.W, **pad)
        self.ent_cycle = ttk.Entry(form, width=12)
        self.ent_cycle.insert(0, "100")
        self.ent_cycle.grid(row=4, column=1, sticky=tk.W, **pad)

        # Extended Frame Checkbox
        self.var_ext = tk.BooleanVar(value=False)
        self.chk_ext = ttk.Checkbutton(form, text="Extended 29-bit CAN ID", variable=self.var_ext)
        self.chk_ext.grid(row=5, column=1, sticky=tk.W, **pad)

        # Comment
        ttk.Label(form, text="Description / Comment:").grid(row=6, column=0, sticky=tk.NW, **pad)
        self.txt_comment = tk.Text(form, width=30, height=4, font=("TkDefaultFont", 9))
        self.txt_comment.grid(row=6, column=1, columnspan=2, sticky=tk.W, **pad)

        # Buttons
        btn_frame = ttk.Frame(form)
        btn_frame.grid(row=7, column=0, columnspan=3, pady=(15, 0), sticky=tk.E)
        ttk.Button(btn_frame, text="Cancel", command=self.destroy).pack(side=tk.RIGHT, padx=5)
        ttk.Button(btn_frame, text="Save" if self.message else "Add", command=self._on_save).pack(side=tk.RIGHT)

    def _populate(self):
        m = self.message
        self.ent_name.insert(0, m.name)
        self.ent_id.insert(0, f"0x{m.frame_id:X}")
        self.spn_length.set(m.length)
        self.cb_sender.set(m.senders[0] if m.senders else "")
        self.ent_cycle.delete(0, tk.END)
        self.ent_cycle.insert(0, str(self.model.get_cycle_time(m)))
        self.var_ext.set(m.is_extended_frame)
        if m.comment:
            self.txt_comment.insert("1.0", m.comment)

    def _on_save(self):
        name = self.ent_name.get().strip()
        raw_id = self.ent_id.get().strip()
        raw_len = self.spn_length.get().strip()
        sender = self.cb_sender.get().strip()
        raw_cycle = self.ent_cycle.get().strip()
        comment = self.txt_comment.get("1.0", tk.END).strip()
        is_ext = self.var_ext.get()

        if not name:
            messagebox.showerror("Error", "Message Name cannot be empty.", parent=self)
            return

        if not re.match(r"^[a-zA-Z_][a-zA-Z0-9_]*$", name):
            messagebox.showerror(
                "Invalid Message Name",
                f"Invalid message name '{name}'.\n\n"
                "Message names must be valid C identifiers: letters, numbers, and underscores only, "
                "and cannot start with a number or contain spaces/punctuation.",
                parent=self
            )
            return

        # Check duplicate name
        for m in self.model.messages:
            if m != self.message and m.name.lower() == name.lower():
                messagebox.showerror("Duplicate Name", f"Message name '{name}' already exists.", parent=self)
                return

        if not raw_id:
            messagebox.showerror("Error", "CAN ID cannot be empty.", parent=self)
            return

        try:
            frame_id = int(raw_id, 0)
        except ValueError:
            messagebox.showerror("Error", f"Invalid CAN ID '{raw_id}'. Use hex (e.g. 0x200) or decimal.", parent=self)
            return

        if frame_id < 0:
            messagebox.showerror("Invalid CAN ID", "CAN ID cannot be negative.", parent=self)
            return

        if frame_id > 0x1FFFFFFF:
            messagebox.showerror(
                "Invalid CAN ID",
                f"CAN ID 0x{frame_id:X} exceeds the maximum 29-bit CAN ID limit (0x1FFFFFFF / 536,870,911).",
                parent=self
            )
            return

        # Check duplicate ID
        for m in self.model.messages:
            if m != self.message and m.frame_id == frame_id:
                messagebox.showerror(
                    "Duplicate CAN ID",
                    f"CAN ID 0x{frame_id:X} is already used by message '{m.name}'.",
                    parent=self
                )
                return

        try:
            length = int(raw_len)
            if not 0 <= length <= 64:
                raise ValueError()
        except ValueError:
            messagebox.showerror("Error", "Length must be between 0 and 64 bytes.", parent=self)
            return

        # If editing an existing message with signals, verify length doesn't break existing signals
        if self.message and self.message.signals:
            try:
                Message(frame_id=frame_id, name=name, length=length, signals=self.message.signals)
            except Exception as err:
                messagebox.showerror(
                    "DLC Reduction Error",
                    f"Cannot reduce length to {length} bytes.\n\n"
                    f"Existing signals in '{self.message.name}' require more bytes:\n{err}",
                    parent=self
                )
                return

        cycle = 0
        if raw_cycle:
            try:
                cycle = int(float(raw_cycle))
                if cycle < 0:
                    raise ValueError()
            except ValueError:
                messagebox.showerror("Error", "Cycle Time must be a non-negative integer in milliseconds.", parent=self)
                return

        self.result = {
            "name": name,
            "frame_id": frame_id,
            "length": length,
            "senders": [sender] if sender else [],
            "cycle_time": cycle,
            "is_extended": is_ext or (frame_id > 0x7FF),
            "comment": comment
        }
        self.destroy()


class SignalDialog(_Toplevel):
    """Modal dialog to create or edit a Signal."""

    def __init__(self, parent, message, signal=None):
        super().__init__(parent)
        self.message = message
        self.signal = signal
        self.result = None

        self.title("Edit Signal" if signal else f"Add Signal to {message.name}")
        self.geometry("450x480")
        self.resizable(False, False)
        self.transient(parent)
        self.grab_set()

        self._build_ui()
        if self.signal:
            self._populate()

    def _build_ui(self):
        pad = {"padx": 10, "pady": 4}
        form = ttk.Frame(self, padding=12)
        form.pack(fill=tk.BOTH, expand=True)

        # Name
        ttk.Label(form, text="Signal Name:").grid(row=0, column=0, sticky=tk.W, **pad)
        self.ent_name = ttk.Entry(form, width=24)
        self.ent_name.grid(row=0, column=1, sticky=tk.W, **pad)

        # Start bit & Length
        ttk.Label(form, text="Start Bit (0-63):").grid(row=1, column=0, sticky=tk.W, **pad)
        self.spn_start = ttk.Spinbox(form, from_=0, to=512, width=6)
        self.spn_start.set(0)
        self.spn_start.grid(row=1, column=1, sticky=tk.W, **pad)

        ttk.Label(form, text="Length (bits):").grid(row=2, column=0, sticky=tk.W, **pad)
        self.spn_len = ttk.Spinbox(form, from_=1, to=64, width=6)
        self.spn_len.set(8)
        self.spn_len.grid(row=2, column=1, sticky=tk.W, **pad)

        # Byte Order
        ttk.Label(form, text="Byte Order:").grid(row=3, column=0, sticky=tk.W, **pad)
        self.cb_order = ttk.Combobox(form, values=["little_endian (Intel)", "big_endian (Motorola)"], width=22)
        self.cb_order.set("little_endian (Intel)")
        self.cb_order.grid(row=3, column=1, sticky=tk.W, **pad)

        # Type (Signed / Unsigned)
        self.var_signed = tk.BooleanVar(value=False)
        self.chk_signed = ttk.Checkbutton(form, text="Signed Integer (two's complement)", variable=self.var_signed)
        self.chk_signed.grid(row=4, column=1, sticky=tk.W, **pad)

        # Scale & Offset
        ttk.Label(form, text="Scale Factor:").grid(row=5, column=0, sticky=tk.W, **pad)
        self.ent_scale = ttk.Entry(form, width=12)
        self.ent_scale.insert(0, "1.0")
        self.ent_scale.grid(row=5, column=1, sticky=tk.W, **pad)

        ttk.Label(form, text="Offset:").grid(row=6, column=0, sticky=tk.W, **pad)
        self.ent_offset = ttk.Entry(form, width=12)
        self.ent_offset.insert(0, "0.0")
        self.ent_offset.grid(row=6, column=1, sticky=tk.W, **pad)

        # Unit
        ttk.Label(form, text="Unit:").grid(row=7, column=0, sticky=tk.W, **pad)
        self.ent_unit = ttk.Entry(form, width=12)
        self.ent_unit.grid(row=7, column=1, sticky=tk.W, **pad)

        # Comment
        ttk.Label(form, text="Comment:").grid(row=8, column=0, sticky=tk.NW, **pad)
        self.txt_comment = tk.Text(form, width=26, height=3, font=("TkDefaultFont", 9))
        self.txt_comment.grid(row=8, column=1, sticky=tk.W, **pad)

        # Buttons
        btn_frame = ttk.Frame(form)
        btn_frame.grid(row=9, column=0, columnspan=2, pady=(12, 0), sticky=tk.E)
        ttk.Button(btn_frame, text="Cancel", command=self.destroy).pack(side=tk.RIGHT, padx=5)
        ttk.Button(btn_frame, text="Save" if self.signal else "Add", command=self._on_save).pack(side=tk.RIGHT)

    def _populate(self):
        s = self.signal
        self.ent_name.insert(0, s.name)
        self.spn_start.set(s.start)
        self.spn_len.set(s.length)
        order_str = "big_endian (Motorola)" if s.byte_order == "big_endian" else "little_endian (Intel)"
        self.cb_order.set(order_str)
        self.var_signed.set(s.is_signed)
        self.ent_scale.delete(0, tk.END)
        self.ent_scale.insert(0, str(s.scale))
        self.ent_offset.delete(0, tk.END)
        self.ent_offset.insert(0, str(s.offset))
        self.ent_unit.insert(0, s.unit or "")
        if s.comment:
            self.txt_comment.insert("1.0", s.comment)

    def _on_save(self):
        name = self.ent_name.get().strip()
        if not name:
            messagebox.showerror("Error", "Signal name cannot be empty.", parent=self)
            return

        if not re.match(r"^[a-zA-Z_][a-zA-Z0-9_]*$", name):
            messagebox.showerror(
                "Invalid Signal Name",
                f"Invalid signal name '{name}'.\n\n"
                "Signal names must be valid C identifiers: letters, numbers, and underscores only, "
                "and cannot start with a number or contain spaces/punctuation.",
                parent=self
            )
            return

        # Check duplicate signal name
        for s in self.message.signals:
            if s != self.signal and s.name.lower() == name.lower():
                messagebox.showerror(
                    "Duplicate Signal Name",
                    f"Signal '{name}' already exists in message '{self.message.name}'.",
                    parent=self
                )
                return

        try:
            start = int(self.spn_start.get())
            length = int(self.spn_len.get())
            scale = float(self.ent_scale.get().strip() or "1.0")
            offset = float(self.ent_offset.get().strip() or "0.0")
        except ValueError as err:
            messagebox.showerror("Error", f"Invalid numerical value: {err}", parent=self)
            return

        if not 1 <= length <= 64:
            messagebox.showerror("Invalid Length", "Signal length must be between 1 and 64 bits.", parent=self)
            return

        if not 0 <= start <= 512:
            messagebox.showerror("Invalid Start Bit", "Signal start bit must be between 0 and 512.", parent=self)
            return

        if scale == 0.0:
            messagebox.showerror(
                "Invalid Scale Factor",
                "Scale factor cannot be 0.0 (would cause divide-by-zero during signal encoding).",
                parent=self
            )
            return

        if math.isnan(scale) or math.isinf(scale):
            messagebox.showerror("Invalid Scale Factor", "Scale factor must be a finite number.", parent=self)
            return

        if math.isnan(offset) or math.isinf(offset):
            messagebox.showerror("Invalid Offset", "Offset must be a finite number.", parent=self)
            return

        order = "big_endian" if "big_endian" in self.cb_order.get() else "little_endian"

        # Pre-validate geometry and fit with cantools
        try:
            test_sig = Signal(name=name, start=start, length=length, byte_order=order, is_signed=self.var_signed.get())
            Message(frame_id=self.message.frame_id, name=self.message.name, length=self.message.length, signals=[test_sig])
        except Exception as err:
            max_bits = self.message.length * 8
            messagebox.showerror(
                "Signal Bounds Error",
                f"Signal '{name}' does not fit in message '{self.message.name}' (DLC: {self.message.length} bytes = {max_bits} bits):\n\n{err}",
                parent=self
            )
            return
        unit = self.ent_unit.get().strip()
        comment = self.txt_comment.get("1.0", tk.END).strip()

        self.result = {
            "name": name,
            "start": start,
            "length": length,
            "byte_order": order,
            "is_signed": self.var_signed.get(),
            "scale": scale,
            "offset": offset,
            "unit": unit,
            "minimum": None,
            "maximum": None,
            "comment": comment
        }
        self.destroy()


# ─────────────────────────────────────────────────────────────────────────────
# Main Application GUI
# ─────────────────────────────────────────────────────────────────────────────

class DbcEditorApp(_Tk):
    """Main window for Vector DBC Visual Editor."""

    def __init__(self, initial_path=None):
        super().__init__()
        self.title("Vector DBC Visual Editor — pico_canlib")
        self.geometry("1100x680")
        self.minsize(800, 500)

        self.model = DbcModel(initial_path)
        self.selected_message = None

        self._init_styles()
        self._build_menu()
        self._build_toolbar()
        self._build_main_view()
        self._build_statusbar()

        self._refresh_messages_table()
        self._update_title()

    def _init_styles(self):
        style = ttk.Style(self)
        style.theme_use("clam")
        style.configure("Treeview", rowheight=24, font=("Segoe UI", 9))
        style.configure("Treeview.Heading", font=("Segoe UI", 9, "bold"))

    def _build_menu(self):
        menubar = tk.Menu(self)
        self.config(menu=menubar)

        # File menu
        file_menu = tk.Menu(menubar, tearoff=0)
        file_menu.add_command(label="New Database", accelerator="Ctrl+N", command=self.on_new)
        file_menu.add_command(label="Open DBC...", accelerator="Ctrl+O", command=self.on_open)
        file_menu.add_command(label="Save (Auto-generates Header)", accelerator="Ctrl+S", command=self.on_save)
        file_menu.add_command(label="Save As...", command=self.on_save_as)
        file_menu.add_separator()
        file_menu.add_command(label="Export C++ Header As...", accelerator="Ctrl+G", command=self.on_generate_header)
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=self.on_exit)
        menubar.add_cascade(label="File", menu=file_menu)

        # Edit menu
        edit_menu = tk.Menu(menubar, tearoff=0)
        edit_menu.add_command(label="Add Message...", accelerator="Ctrl+M", command=self.on_add_message)
        edit_menu.add_command(label="Edit Message...", command=self.on_edit_message)
        edit_menu.add_command(label="Delete Message", accelerator="Delete", command=self.on_delete_message)
        edit_menu.add_separator()
        edit_menu.add_command(label="Add Signal...", command=self.on_add_signal)
        edit_menu.add_command(label="Edit Signal...", command=self.on_edit_signal)
        edit_menu.add_command(label="Delete Signal", command=self.on_delete_signal)
        menubar.add_cascade(label="Edit", menu=edit_menu)

        # Help menu
        help_menu = tk.Menu(menubar, tearoff=0)
        help_menu.add_command(label="About", command=self.on_about)
        menubar.add_cascade(label="Help", menu=help_menu)

        # Shortcuts
        self.bind("<Control-n>", lambda e: self.on_new())
        self.bind("<Control-o>", lambda e: self.on_open())
        self.bind("<Control-s>", lambda e: self.on_save())
        self.bind("<Control-g>", lambda e: self.on_generate_header())
        self.bind("<Control-m>", lambda e: self.on_add_message())
        self.bind("<Delete>", lambda e: self.on_delete_key())

    def _build_toolbar(self):
        toolbar = ttk.Frame(self, padding=(6, 4))
        toolbar.pack(side=tk.TOP, fill=tk.X)

        ttk.Button(toolbar, text="📂 Open", command=self.on_open).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="💾 Save", command=self.on_save).pack(side=tk.LEFT, padx=2)
        ttk.Separator(toolbar, orient=tk.VERTICAL).pack(side=tk.LEFT, fill=tk.Y, padx=6)

        ttk.Button(toolbar, text="➕ Add Message", command=self.on_add_message).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="✏️ Edit Message", command=self.on_edit_message).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="🗑️ Delete Message", command=self.on_delete_message).pack(side=tk.LEFT, padx=2)
        ttk.Separator(toolbar, orient=tk.VERTICAL).pack(side=tk.LEFT, fill=tk.Y, padx=6)

        # Search / Filter Box
        ttk.Label(toolbar, text="🔍 Search:").pack(side=tk.LEFT, padx=(6, 2))
        self.var_filter = tk.StringVar()
        self.var_filter.trace_add("write", lambda *args: self._refresh_messages_table())
        ent_search = ttk.Entry(toolbar, textvariable=self.var_filter, width=22)
        ent_search.pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="✕", width=2, command=lambda: self.var_filter.set("")).pack(side=tk.LEFT)

    def _build_main_view(self):
        paned = ttk.PanedWindow(self, orient=tk.HORIZONTAL)
        paned.pack(fill=tk.BOTH, expand=True, padx=6, pady=4)

        # ── Left Pane: CAN Messages Table ────────────────────────────────────
        left_frame = ttk.LabelFrame(paned, text="CAN Messages", padding=6)
        paned.add(left_frame, weight=3)

        cols = ("id_hex", "id_dec", "name", "dlc", "sender", "cycle", "signals")
        self.tree_msgs = ttk.Treeview(left_frame, columns=cols, show="headings", selectmode="browse")
        self.tree_msgs.heading("id_hex", text="ID (Hex)")
        self.tree_msgs.heading("id_dec", text="ID (Dec)")
        self.tree_msgs.heading("name", text="Message Name")
        self.tree_msgs.heading("dlc", text="DLC")
        self.tree_msgs.heading("sender", text="Sender")
        self.tree_msgs.heading("cycle", text="Cycle (ms)")
        self.tree_msgs.heading("signals", text="Signals")

        self.tree_msgs.column("id_hex", width=75, anchor=tk.CENTER)
        self.tree_msgs.column("id_dec", width=65, anchor=tk.CENTER)
        self.tree_msgs.column("name", width=160, anchor=tk.W)
        self.tree_msgs.column("dlc", width=45, anchor=tk.CENTER)
        self.tree_msgs.column("sender", width=110, anchor=tk.W)
        self.tree_msgs.column("cycle", width=70, anchor=tk.CENTER)
        self.tree_msgs.column("signals", width=55, anchor=tk.CENTER)

        scroll_y = ttk.Scrollbar(left_frame, orient=tk.VERTICAL, command=self.tree_msgs.yview)
        self.tree_msgs.configure(yscrollcommand=scroll_y.set)

        self.tree_msgs.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scroll_y.pack(side=tk.RIGHT, fill=tk.Y)

        self.tree_msgs.bind("<<TreeviewSelect>>", self._on_message_selected)
        self.tree_msgs.bind("<Double-1>", lambda e: self.on_edit_message())

        # ── Right Pane: Signal Inspector & Table ─────────────────────────────
        right_frame = ttk.LabelFrame(paned, text="Signals in Selected Message", padding=6)
        paned.add(right_frame, weight=4)

        # Top card with message details
        self.lbl_msg_info = ttk.Label(right_frame, text="Select a message to view its signals.", font=("Segoe UI", 9, "bold"))
        self.lbl_msg_info.pack(anchor=tk.W, pady=(0, 4))

        # Signal Table Container
        tree_frame = ttk.Frame(right_frame)
        tree_frame.pack(fill=tk.BOTH, expand=True)

        sig_cols = ("name", "start", "length", "order", "type", "scale", "offset", "unit")
        self.tree_sigs = ttk.Treeview(tree_frame, columns=sig_cols, show="headings", selectmode="browse")
        self.tree_sigs.heading("name", text="Signal Name")
        self.tree_sigs.heading("start", text="Start")
        self.tree_sigs.heading("length", text="Length")
        self.tree_sigs.heading("order", text="Order")
        self.tree_sigs.heading("type", text="Type")
        self.tree_sigs.heading("scale", text="Scale")
        self.tree_sigs.heading("offset", text="Offset")
        self.tree_sigs.heading("unit", text="Unit")

        self.tree_sigs.column("name", width=140, anchor=tk.W)
        self.tree_sigs.column("start", width=45, anchor=tk.CENTER)
        self.tree_sigs.column("length", width=50, anchor=tk.CENTER)
        self.tree_sigs.column("order", width=70, anchor=tk.CENTER)
        self.tree_sigs.column("type", width=55, anchor=tk.CENTER)
        self.tree_sigs.column("scale", width=55, anchor=tk.CENTER)
        self.tree_sigs.column("offset", width=50, anchor=tk.CENTER)
        self.tree_sigs.column("unit", width=50, anchor=tk.CENTER)

        scroll_sig_y = ttk.Scrollbar(tree_frame, orient=tk.VERTICAL, command=self.tree_sigs.yview)
        self.tree_sigs.configure(yscrollcommand=scroll_sig_y.set)

        self.tree_sigs.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scroll_sig_y.pack(side=tk.RIGHT, fill=tk.Y)

        self.tree_sigs.bind("<Double-1>", lambda e: self.on_edit_signal())
        self.tree_sigs.bind("<<TreeviewSelect>>", self._on_signal_selected)

        # Signal Action Toolbar
        sig_btn_bar = ttk.Frame(right_frame, padding=(0, 4))
        sig_btn_bar.pack(fill=tk.X)
        ttk.Button(sig_btn_bar, text="➕ Add Signal", command=self.on_add_signal).pack(side=tk.LEFT, padx=2)
        ttk.Button(sig_btn_bar, text="✏️ Edit Signal", command=self.on_edit_signal).pack(side=tk.LEFT, padx=2)
        ttk.Button(sig_btn_bar, text="🗑️ Delete Signal", command=self.on_delete_signal).pack(side=tk.LEFT, padx=2)

        # Comment / Notes box at bottom (raised height with dedicated scrollbar)
        ttk.Label(right_frame, text="Signal / Message Notes:", font=("Segoe UI", 9, "bold")).pack(anchor=tk.W, pady=(8, 2))
        notes_frame = ttk.Frame(right_frame)
        notes_frame.pack(fill=tk.X, pady=(0, 2))

        self.txt_inspector_comment = tk.Text(
            notes_frame,
            height=6,
            font=("Segoe UI", 9),
            wrap=tk.WORD,
            state=tk.DISABLED
        )
        scroll_notes_y = ttk.Scrollbar(notes_frame, orient=tk.VERTICAL, command=self.txt_inspector_comment.yview)
        self.txt_inspector_comment.configure(yscrollcommand=scroll_notes_y.set)

        self.txt_inspector_comment.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scroll_notes_y.pack(side=tk.RIGHT, fill=tk.Y)

    def _build_statusbar(self):
        self.statusbar = ttk.Label(self, text="Ready", relief=tk.SUNKEN, anchor=tk.W, padding=4)
        self.statusbar.pack(side=tk.BOTTOM, fill=tk.X)

    # ── Table Refresh Helpers ────────────────────────────────────────────────

    def _refresh_messages_table(self):
        selected_id = self.selected_message.frame_id if self.selected_message else None

        for item in self.tree_msgs.get_children():
            self.tree_msgs.delete(item)

        filter_text = self.var_filter.get().strip().lower()

        reselect_item = None
        count = 0
        for m in self.model.messages:
            id_hex = f"0x{m.frame_id:X}"
            id_dec = str(m.frame_id)
            sender = m.senders[0] if m.senders else "Vector__XXX"
            cycle = str(self.model.get_cycle_time(m))

            # Filtering
            if filter_text:
                match = (filter_text in m.name.lower() or
                         filter_text in id_hex.lower() or
                         filter_text in id_dec or
                         filter_text in sender.lower())
                if not match:
                    continue

            item = self.tree_msgs.insert("", tk.END, values=(
                id_hex, id_dec, m.name, m.length, sender, cycle, len(m.signals)
            ))
            count += 1
            if selected_id and m.frame_id == selected_id:
                reselect_item = item

        if reselect_item:
            self.tree_msgs.selection_set(reselect_item)
            self.tree_msgs.see(reselect_item)
        elif self.tree_msgs.get_children():
            first = self.tree_msgs.get_children()[0]
            self.tree_msgs.selection_set(first)

        msg_str = f"{count} message{'s' if count != 1 else ''}"
        path_str = str(self.model.filepath) if self.model.filepath else "Untitled"
        self.statusbar.config(text=f"{path_str} — {msg_str}")

    def _refresh_signals_table(self):
        for item in self.tree_sigs.get_children():
            self.tree_sigs.delete(item)

        if not self.selected_message:
            self.lbl_msg_info.config(text="Select a message to view its signals.")
            self._update_inspector_comment("")
            return

        m = self.selected_message
        cycle = self.model.get_cycle_time(m)
        cycle_str = f" | Period: {cycle} ms" if cycle else ""
        ext_str = " (29-bit Extended)" if m.is_extended_frame else ""
        self.lbl_msg_info.config(
            text=f"0x{m.frame_id:X} — {m.name} [DLC: {m.length} bytes{ext_str}{cycle_str}]"
        )
        self._update_inspector_comment(m.comment or "")

        for s in m.signals:
            order = "Intel (LE)" if s.byte_order == "little_endian" else "Motorola (BE)"
            sig_type = "Signed" if s.is_signed else "Unsigned"
            self.tree_sigs.insert("", tk.END, values=(
                s.name, s.start, s.length, order, sig_type, s.scale, s.offset, s.unit
            ))

    def _update_inspector_comment(self, text):
        self.txt_inspector_comment.config(state=tk.NORMAL)
        self.txt_inspector_comment.delete("1.0", tk.END)
        self.txt_inspector_comment.insert("1.0", text)
        self.txt_inspector_comment.config(state=tk.DISABLED)

    def _on_message_selected(self, event):
        sel = self.tree_msgs.selection()
        if not sel:
            self.selected_message = None
            self._refresh_signals_table()
            return

        vals = self.tree_msgs.item(sel[0], "values")
        frame_id = int(vals[0], 16)
        self.selected_message = next((m for m in self.model.messages if m.frame_id == frame_id), None)
        self._refresh_signals_table()

    def _on_signal_selected(self, event=None):
        if not self.selected_message:
            return
        sel = self.tree_sigs.selection()
        if not sel:
            self._update_inspector_comment(self.selected_message.comment or "")
            return
        sig_name = self.tree_sigs.item(sel[0], "values")[0]
        sig = next((s for s in self.selected_message.signals if s.name == sig_name), None)
        if not sig:
            return

        notes = []
        if sig.comment:
            notes.append(f"Signal '{sig.name}': {sig.comment}")
        else:
            notes.append(f"Signal '{sig.name}'")

        if hasattr(sig, "choices") and sig.choices:
            notes.append("\nChoices / State Bits:")
            for val, desc in sorted(sig.choices.items(), key=lambda x: x[0]):
                notes.append(f"  [{val}] = {desc}")

        if self.selected_message.comment:
            notes.append(f"\nMessage Note ({self.selected_message.name}):\n{self.selected_message.comment}")

        self._update_inspector_comment("\n".join(notes))

    def _update_title(self):
        dirty = "*" if self.model.is_dirty else ""
        fname = self.model.filepath.name if self.model.filepath else "Untitled.dbc"
        self.title(f"{dirty}{fname} — Vector DBC Visual Editor")

    # ── Menu & Button Commands ───────────────────────────────────────────────

    def on_new(self):
        if self._check_unsaved_changes():
            self.model.new_database()
            self.selected_message = None
            self._refresh_messages_table()
            self._refresh_signals_table()
            self._update_title()

    def on_open(self):
        if not self._check_unsaved_changes():
            return
        path = filedialog.askopenfilename(
            parent=self,
            title="Open Vector DBC File",
            filetypes=[("Vector DBC Files", "*.dbc"), ("All Files", "*.*")]
        )
        if path:
            try:
                self.model.load(path)
                self.selected_message = None
                self._refresh_messages_table()
                self._refresh_signals_table()
                self._update_title()
            except Exception as err:
                messagebox.showerror("Open Error", f"Failed to open DBC file:\n{err}", parent=self)

    def on_save(self):
        if not self.model.filepath:
            return self.on_save_as()
        try:
            hdr_path = self.model.save(auto_generate_header=True)
            self._update_title()
            if hdr_path:
                self.statusbar.config(text=f"Saved {self.model.filepath.name} & updated {hdr_path.name}")
            else:
                self.statusbar.config(text=f"Saved to {self.model.filepath}")
        except Exception as err:
            messagebox.showerror("Save Error", f"Failed to save DBC file:\n{err}", parent=self)

    def on_save_as(self):
        initial = str(self.model.filepath) if self.model.filepath else "can_database.dbc"
        path = filedialog.asksaveasfilename(
            parent=self,
            title="Save Vector DBC As",
            initialfile=Path(initial).name,
            defaultextension=".dbc",
            filetypes=[("Vector DBC Files", "*.dbc"), ("All Files", "*.*")]
        )
        if path:
            try:
                hdr_path = self.model.save(filepath=path, auto_generate_header=True)
                self._update_title()
                if hdr_path:
                    self.statusbar.config(text=f"Saved as {Path(path).name} & updated {hdr_path.name}")
                else:
                    self.statusbar.config(text=f"Saved as {path}")
            except Exception as err:
                messagebox.showerror("Save Error", f"Failed to save DBC file:\n{err}", parent=self)

    def on_generate_header(self):
        if not self.model.messages:
            messagebox.showwarning(
                "Empty Database",
                "Cannot generate C++ header: The CAN database contains no messages.",
                parent=self
            )
            return

        default_dir = PROJECT_ROOT / "Inc"
        target_str = filedialog.asksaveasfilename(
            parent=self,
            title="Generate C++ CAN Header",
            initialdir=str(default_dir if default_dir.is_dir() else PROJECT_ROOT),
            initialfile="artemis_canid.hpp",
            defaultextension=".hpp",
            filetypes=[("C++ Header Files", "*.hpp *.h"), ("All Files", "*.*")]
        )
        if not target_str:
            return

        try:
            target_path = self.model.generate_header(output_path=target_str)
            msg_count = len(self.model.messages)
            messagebox.showinfo(
                "Header Generated",
                f"Successfully generated C++ CAN header:\n\n{target_path}\n\n({msg_count} messages compiled)",
                parent=self
            )
            self.statusbar.config(text=f"Generated {target_path.name} ({msg_count} messages)")
        except Exception as err:
            messagebox.showerror("Header Generation Error", f"Failed to generate header:\n{err}", parent=self)

    def _check_unsaved_changes(self):
        if self.model.is_dirty:
            resp = messagebox.askyesnocancel(
                "Unsaved Changes",
                "You have unsaved changes in this database. Do you want to save them before continuing?",
                parent=self
            )
            if resp is True:
                self.on_save()
                return True
            elif resp is False:
                return True
            return False
        return True

    def on_exit(self):
        if self._check_unsaved_changes():
            self.destroy()

    # ── CAN Message Operations (Add, Edit, Remove) ───────────────────────────

    def on_add_message(self):
        dlg = MessageDialog(self, self.model)
        self.wait_window(dlg)
        if dlg.result:
            try:
                msg = self.model.add_message(
                    frame_id=dlg.result["frame_id"],
                    name=dlg.result["name"],
                    length=dlg.result["length"],
                    senders=dlg.result["senders"],
                    comment=dlg.result["comment"],
                    cycle_time=dlg.result["cycle_time"],
                    is_extended=dlg.result["is_extended"]
                )
                self.selected_message = msg
                self._refresh_messages_table()
                self._refresh_signals_table()
                self._update_title()
            except Exception as err:
                messagebox.showerror("Add Message Error", str(err), parent=self)

    def on_edit_message(self):
        if not self.selected_message:
            messagebox.showinfo("Edit Message", "Please select a message to edit.", parent=self)
            return

        dlg = MessageDialog(self, self.model, message=self.selected_message)
        self.wait_window(dlg)
        if dlg.result:
            try:
                self.model.edit_message(
                    orig_msg=self.selected_message,
                    frame_id=dlg.result["frame_id"],
                    name=dlg.result["name"],
                    length=dlg.result["length"],
                    senders=dlg.result["senders"],
                    comment=dlg.result["comment"],
                    cycle_time=dlg.result["cycle_time"],
                    is_extended=dlg.result["is_extended"]
                )
                self._refresh_messages_table()
                self._refresh_signals_table()
                self._update_title()
            except Exception as err:
                messagebox.showerror("Edit Message Error", str(err), parent=self)

    def on_delete_message(self):
        if not self.selected_message:
            messagebox.showinfo("Delete Message", "Please select a message to delete.", parent=self)
            return

        m = self.selected_message
        sig_count = len(m.signals)
        sig_warning = f" and all of its {sig_count} signals" if sig_count > 0 else ""

        confirm = messagebox.askyesno(
            "Confirm Delete Message",
            f"Are you sure you want to delete message '{m.name}' (0x{m.frame_id:X}){sig_warning}?\n\nThis cannot be undone.",
            icon=messagebox.WARNING,
            parent=self
        )
        if confirm:
            self.model.remove_message(m)
            self.selected_message = None
            self._refresh_messages_table()
            self._refresh_signals_table()
            self._update_title()

    def on_delete_key(self):
        focus = self.focus_get()
        if focus == self.tree_sigs:
            self.on_delete_signal()
        else:
            self.on_delete_message()

    # ── Signal Operations ────────────────────────────────────────────────────

    def on_add_signal(self):
        if not self.selected_message:
            messagebox.showinfo("Add Signal", "Please select a message first.", parent=self)
            return

        dlg = SignalDialog(self, self.selected_message)
        self.wait_window(dlg)
        if dlg.result:
            try:
                self.model.add_signal(
                    message=self.selected_message,
                    name=dlg.result["name"],
                    start=dlg.result["start"],
                    length=dlg.result["length"],
                    byte_order=dlg.result["byte_order"],
                    is_signed=dlg.result["is_signed"],
                    scale=dlg.result["scale"],
                    offset=dlg.result["offset"],
                    unit=dlg.result["unit"],
                    minimum=dlg.result["minimum"],
                    maximum=dlg.result["maximum"],
                    comment=dlg.result["comment"]
                )
                self._refresh_signals_table()
                self._refresh_messages_table()
                self._update_title()
            except Exception as err:
                messagebox.showerror("Add Signal Error", str(err), parent=self)

    def on_edit_signal(self):
        if not self.selected_message:
            return
        sel = self.tree_sigs.selection()
        if not sel:
            messagebox.showinfo("Edit Signal", "Please select a signal to edit.", parent=self)
            return

        sig_name = self.tree_sigs.item(sel[0], "values")[0]
        sig = next((s for s in self.selected_message.signals if s.name == sig_name), None)
        if not sig:
            return

        dlg = SignalDialog(self, self.selected_message, signal=sig)
        self.wait_window(dlg)
        if dlg.result:
            try:
                self.model.edit_signal(
                    message=self.selected_message,
                    orig_sig=sig,
                    name=dlg.result["name"],
                    start=dlg.result["start"],
                    length=dlg.result["length"],
                    byte_order=dlg.result["byte_order"],
                    is_signed=dlg.result["is_signed"],
                    scale=dlg.result["scale"],
                    offset=dlg.result["offset"],
                    unit=dlg.result["unit"],
                    minimum=dlg.result["minimum"],
                    maximum=dlg.result["maximum"],
                    comment=dlg.result["comment"]
                )
                self._refresh_signals_table()
                self._update_title()
            except Exception as err:
                messagebox.showerror("Edit Signal Error", str(err), parent=self)

    def on_delete_signal(self):
        if not self.selected_message:
            return
        sel = self.tree_sigs.selection()
        if not sel:
            messagebox.showinfo("Delete Signal", "Please select a signal to delete.", parent=self)
            return

        sig_name = self.tree_sigs.item(sel[0], "values")[0]
        sig = next((s for s in self.selected_message.signals if s.name == sig_name), None)
        if not sig:
            return

        confirm = messagebox.askyesno(
            "Confirm Delete Signal",
            f"Are you sure you want to delete signal '{sig.name}' from message '{self.selected_message.name}'?",
            parent=self
        )
        if confirm:
            self.model.remove_signal(self.selected_message, sig)
            self._refresh_signals_table()
            self._refresh_messages_table()
            self._update_title()

    def on_about(self):
        messagebox.showinfo(
            "About Vector DBC Visual Editor",
            "Vector DBC Visual Editor\n"
            "Part of the pico_canlib embedded CAN toolchain.\n\n"
            "• Supports Vector .dbc file creation and modification\n"
            "• Add, edit, duplicate, and remove CAN messages\n"
            "• Configure signal bit offsets, scaling, units, and cycle times\n"
            "• Integrates with toHeader.py for C++ firmware code generation",
            parent=self
        )


# ─────────────────────────────────────────────────────────────────────────────
# CLI Entry Point
# ─────────────────────────────────────────────────────────────────────────────

def main():
    # This causes the fuzziness on Windows to disappear because of DPI issues.
    # However, it also cauzes sizing issues
    # if sys.platform == "win32":
    #    try:
    #        import ctypes
    #        ctypes.windll.shcore.SetProcessDpiAwareness(2)
    #    except Exception:
    #        try:
    #            ctypes.windll.user32.SetProcessDPIAware()
    #        except Exception:
    #            pass
    # 

    parser = argparse.ArgumentParser(description="Vector DBC Visual Editor for CAN Databases.")
    parser.add_argument("dbc_file", nargs="?", default=None, help="Optional DBC file to open or compile.")
    parser.add_argument(
        "--header",
        nargs="?",
        const="Inc/artemis_canid.hpp",
        default=None,
        help="Compile C++ header directly without launching GUI (default: Inc/artemis_canid.hpp)"
    )
    args = parser.parse_args()

    if args.dbc_file and not Path(args.dbc_file).is_file():
        sys.exit(f"error: DBC file '{args.dbc_file}' not found.")

    # Headless CLI header generation mode
    if args.header:
        if not args.dbc_file:
            sys.exit("error: Must specify a DBC file when using --header.")
        model = DbcModel(args.dbc_file)
        out_path = model.generate_header(args.header)
        print(f"Generated header: {args.dbc_file} -> {out_path}")
        return

    if tk is None:
        sys.exit(
            "error: 'tkinter' is required to run the visual editor GUI.\n"
            "  • Windows & macOS: Re-run the Python installer and ensure 'tcl/tk' is checked.\n"
            "  • Ubuntu/Debian:   sudo apt install python3-tk\n"
            "  • Fedora/RHEL:     sudo dnf install python3-tkinter\n"
            "  • Arch Linux:      sudo pacman -S tk"
        )

    app = DbcEditorApp(initial_path=args.dbc_file)
    app.protocol("WM_DELETE_WINDOW", app.on_exit)
    app.mainloop()


if __name__ == "__main__":
    main()
