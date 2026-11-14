#!/usr/bin/env python3
"""
test_dbc_editor.py - Automated unit tests for tools/dbcEditor.py DbcModel.

Verifies:
1. Loading an existing complex DBC (tests/PSR_Artemis27.dbc).
2. Adding new messages and signals.
3. Editing message and signal properties.
4. Removing messages and signals.
5. Cycle time (GenMsgCycleTime) handling.
6. Validation rules (duplicate IDs, duplicate names).
7. Creating a database from scratch, saving to disk, and roundtrip reloading.
"""

import sys
import unittest
from pathlib import Path

# Add project root to sys.path
PROJECT_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(PROJECT_ROOT / "tools"))

from dbcEditor import DbcModel
import cantools


class TestDbcModel(unittest.TestCase):
    def setUp(self):
        self.dbc_path = PROJECT_ROOT / "tools" / "PSR Artemis27.dbc"
        if not self.dbc_path.exists():
            self.dbc_path = PROJECT_ROOT / "tests" / "PSR_Artemis27.dbc"
        self.tmp_output = PROJECT_ROOT / "tests" / "temp_test_output.dbc"
        self.tmp_header = PROJECT_ROOT / "tests" / "temp_header.hpp"

    def tearDown(self):
        if self.tmp_output.exists():
            self.tmp_output.unlink()
        if self.tmp_header.exists():
            self.tmp_header.unlink()

    def test_load_existing_dbc(self):
        """Verify that PSR_Artemis27.dbc loads without error under strict=False."""
        self.assertTrue(self.dbc_path.exists(), f"Missing test fixture: {self.dbc_path}")
        model = DbcModel(self.dbc_path)
        self.assertIsNotNone(model.db)
        self.assertGreater(len(model.messages), 10)
        self.assertIn("OrionBMS2", model.nodes or [])

    def test_add_and_remove_message(self):
        """Test adding a new message with signals and then removing it."""
        model = DbcModel(self.dbc_path)
        initial_count = len(model.messages)

        # 1. Add message
        new_msg = model.add_message(
            frame_id=0x555,
            name="TelemetryBroadcast",
            length=8,
            senders=["OrionBMS2"],
            comment="Unit test broadcast frame",
            cycle_time=50,
            is_extended=False
        )
        self.assertEqual(len(model.messages), initial_count + 1)
        self.assertEqual(new_msg.name, "TelemetryBroadcast")
        self.assertEqual(new_msg.frame_id, 0x555)
        self.assertEqual(model.get_cycle_time(new_msg), 50)
        self.assertTrue(model.is_dirty)

        # 2. Add signals
        sig1 = model.add_signal(
            message=new_msg,
            name="VehicleSpeed",
            start=0,
            length=16,
            byte_order="little_endian",
            is_signed=False,
            scale=0.01,
            offset=0.0,
            unit="km/h",
            comment="Speed in km/h"
        )
        sig2 = model.add_signal(
            message=new_msg,
            name="EngineRPM",
            start=16,
            length=16,
            byte_order="little_endian",
            is_signed=False,
            scale=1.0,
            offset=0.0,
            unit="rpm"
        )
        self.assertEqual(len(new_msg.signals), 2)
        self.assertEqual(new_msg.signals[0].name, "VehicleSpeed")
        self.assertEqual(new_msg.signals[0].unit, "km/h")

        # 3. Save to temp DBC and reload
        model.save(self.tmp_output)
        self.assertFalse(model.is_dirty)

        reloaded = DbcModel(self.tmp_output)
        msg_found = next((m for m in reloaded.messages if m.frame_id == 0x555), None)
        self.assertIsNotNone(msg_found, "Added message not found after save/reload")
        self.assertEqual(msg_found.name, "TelemetryBroadcast")
        self.assertEqual(len(msg_found.signals), 2)
        self.assertEqual(reloaded.get_cycle_time(msg_found), 50)

        # 4. Remove message
        reloaded.remove_message(msg_found)
        self.assertEqual(len(reloaded.messages), initial_count)
        self.assertTrue(reloaded.is_dirty)

    def test_edit_message(self):
        """Test editing an existing message's metadata and cycle time."""
        model = DbcModel(self.dbc_path)
        first_msg = model.messages[0]
        original_name = first_msg.name

        model.edit_message(
            orig_msg=first_msg,
            frame_id=first_msg.frame_id,
            name=original_name + "_Renamed",
            length=8,
            senders=first_msg.senders,
            comment="Updated comment",
            cycle_time=123,
            is_extended=first_msg.is_extended_frame
        )

        self.assertEqual(first_msg.name, original_name + "_Renamed")
        self.assertEqual(first_msg.comment, "Updated comment")
        self.assertEqual(model.get_cycle_time(first_msg), 123)
        self.assertTrue(model.is_dirty)

    def test_duplicate_validation(self):
        """Verify that duplicate message IDs and names are rejected."""
        model = DbcModel(self.dbc_path)
        existing = model.messages[0]

        # Duplicate ID
        with self.assertRaises(ValueError):
            model.add_message(
                frame_id=existing.frame_id,
                name="UniqueNameXYZ",
                length=8,
                senders=[]
            )

        # Duplicate Name
        with self.assertRaises(ValueError):
            model.add_message(
                frame_id=0x7FF,
                name=existing.name,
                length=8,
                senders=[]
            )

    def test_signal_crud(self):
        """Test adding, editing, and removing signals."""
        model = DbcModel()  # Brand new empty DB
        msg = model.add_message(frame_id=0x100, name="TestMsg", length=8, senders=[])

        # Add signal
        sig = model.add_signal(msg, name="StatusFlag", start=0, length=1)
        self.assertEqual(len(msg.signals), 1)

        # Duplicate signal in same message should raise ValueError
        with self.assertRaises(ValueError):
            model.add_signal(msg, name="StatusFlag", start=1, length=1)

        # Edit signal
        model.edit_signal(msg, sig, name="StatusFlag_Edited", start=2, length=2, unit="V")
        self.assertEqual(sig.name, "StatusFlag_Edited")
        self.assertEqual(sig.start, 2)
        self.assertEqual(sig.length, 2)
        self.assertEqual(sig.unit, "V")

        # Remove signal
        model.remove_signal(msg, sig)
        self.assertEqual(len(msg.signals), 0)

    def test_empty_database_roundtrip(self):
        """Verify creating a database from scratch, saving, and reopening."""
        model = DbcModel()
        msg = model.add_message(frame_id=0x123, name="Sensors", length=4, senders=["ECU"], cycle_time=10)
        model.add_signal(msg, name="Temp", start=0, length=16, scale=0.1, unit="C")

        model.save(self.tmp_output)
        self.assertTrue(self.tmp_output.exists())

        reloaded = DbcModel(self.tmp_output)
        self.assertEqual(len(reloaded.messages), 1)
        r_msg = reloaded.messages[0]
        self.assertEqual(r_msg.frame_id, 0x123)
        self.assertEqual(r_msg.name, "Sensors")
        self.assertEqual(len(r_msg.signals), 1)
        self.assertEqual(r_msg.signals[0].name, "Temp")
        self.assertEqual(r_msg.signals[0].unit, "C")
        self.assertAlmostEqual(r_msg.signals[0].scale, 0.1)

    def test_generate_header(self):
        """Verify that DbcModel can directly compile a C++ firmware header."""
        model = DbcModel(self.dbc_path)
        out = model.generate_header(self.tmp_header)
        self.assertTrue(out.exists())
        self.assertGreater(out.stat().st_size, 1000)

        content = out.read_text(encoding="utf-8")
        self.assertIn("#pragma once", content)
        self.assertIn("enum class MessageID", content)
        self.assertIn("inline constexpr MessageDefinition artemis_messages[]", content)
        self.assertIn("template<typename SignalEnum> struct MessageOf;", content)

    def test_invalid_bounds_rejected_and_rolled_back(self):
        """Verify that signals with invalid bounds are rejected and safely rolled back."""
        model = DbcModel()
        msg = model.add_message(frame_id=0x123, name="Sensors", length=8, senders=["ECU"])

        # Attempting start=63, length=64 on an 8-byte frame must raise ValueError and roll back
        with self.assertRaises(ValueError) as ctx:
            model.add_signal(msg, name="OverflowSig", start=63, length=64, byte_order="little_endian")

        self.assertIn("does not fit in message", str(ctx.exception))
        # Ensure message signals list was safely rolled back and has 0 items
        self.assertEqual(len(msg.signals), 0)

    def test_message_resizing_bounds_protection(self):
        """Verify that shrinking message DLC below existing signals is rejected and rolled back."""
        model = DbcModel()
        msg = model.add_message(frame_id=0x123, name="Sensors", length=8, senders=["ECU"])
        # Add a signal occupying bytes 6-7 (start=48, length=16)
        model.add_signal(msg, name="TailSignal", start=48, length=16)

        # Attempt to shrink DLC to 4 bytes: must fail and rollback length to 8
        with self.assertRaises(ValueError) as ctx:
            model.edit_message(msg, frame_id=0x123, name="Sensors", length=4, senders=["ECU"])

        self.assertIn("Cannot resize message", str(ctx.exception))
        self.assertEqual(msg.length, 8)

    def test_auto_generate_header_on_save(self):
        """Verify that saving with auto_generate_header=True compiles the C++ header."""
        model = DbcModel(self.dbc_path)
        hdr_path = model.save(
            filepath=self.tmp_output,
            auto_generate_header=True,
            header_path=self.tmp_header
        )
        self.assertTrue(self.tmp_output.exists())
        self.assertTrue(self.tmp_header.exists())
        self.assertEqual(hdr_path, self.tmp_header)
        content = self.tmp_header.read_text(encoding="utf-8")
        self.assertIn("#pragma once", content)
        self.assertIn("enum class MessageID", content)


if __name__ == "__main__":
    unittest.main()
