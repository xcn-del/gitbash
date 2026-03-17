import tkinter as tk
from tkinter import font, messagebox
import re

class SimpleCalculator:
    # Configuration constants
    WINDOW_WIDTH = 400
    WINDOW_HEIGHT = 500
    DISPLAY_FONT = ("Arial", 24)
    BUTTON_FONT_SIZE = 18
    FRAME_PADDING = 10
    BUTTON_PADDING = 5
    DISPLAY_PADDING = 20

    def __init__(self, root):
        self.root = root
        self._setup_window()
        self._create_display()
        self._create_buttons()

    def _setup_window(self):
        self.root.title("Simple Calculator")
        self.root.geometry(f"{self.WINDOW_WIDTH}x{self.WINDOW_HEIGHT}")
        self.root.resizable(False, False)

    def _create_display(self):
        self.display = tk.Entry(
            self.root,
            font=self.DISPLAY_FONT,
            borderwidth=2,
            relief="solid",
            justify="right"
        )
        self.display.pack(fill="both", padx=self.FRAME_PADDING, pady=self.DISPLAY_PADDING, ipady=self.DISPLAY_PADDING)

    def _create_buttons(self):
        button_frame = tk.Frame(self.root)
        button_frame.pack(fill="both", expand=True, padx=self.FRAME_PADDING, pady=self.FRAME_PADDING)

        buttons = [
            ["7", "8", "9", "/"],
            ["4", "5", "6", "*"],
            ["1", "2", "3", "-"],
            ["0", ".", "=", "+"],
            ["C"]
        ]

        self.button_font = font.Font(size=self.BUTTON_FONT_SIZE, weight="bold")

        for row_idx, row in enumerate(buttons):
            for col_idx, btn_text in enumerate(row):
                btn = tk.Button(
                    button_frame,
                    text=btn_text,
                    font=self.button_font,
                    command=lambda x=btn_text: self.on_button_click(x)
                )

                if btn_text == "C":
                    btn.grid(row=row_idx, column=0, columnspan=4, sticky="nsew", padx=self.BUTTON_PADDING, pady=self.BUTTON_PADDING)
                else:
                    btn.grid(row=row_idx, column=col_idx, sticky="nsew", padx=self.BUTTON_PADDING, pady=self.BUTTON_PADDING)

        # Configure grid weights once after all buttons are created
        for row_idx in range(len(buttons)):
            button_frame.grid_rowconfigure(row_idx, weight=1)
        for col_idx in range(4):
            button_frame.grid_columnconfigure(col_idx, weight=1)

    def _clear_display(self):
        self.display.delete(0, tk.END)

    def _set_display(self, value):
        self._clear_display()
        self.display.insert(0, str(value))

    def _append_to_display(self, char):
        self.display.insert(tk.END, char)

    def _safe_eval(self, expression):
        """Safely evaluate a mathematical expression using regex and operator module."""
        try:
            # Only allow digits, operators, and decimal points
            if not re.match(r'^[\d+\-*/.() ]+$', expression):
                raise ValueError("Invalid characters in expression")

            # Use eval with restricted namespace (still evaluate carefully)
            # For production, use a proper expression parser library
            result = eval(expression, {"__builtins__": {}})
            return float(result)
        except (SyntaxError, ValueError, NameError, TypeError, ZeroDivisionError) as e:
            raise ValueError(f"Invalid expression") from e

    def on_button_click(self, char):
        if char == "C":
            self._clear_display()
        elif char == "=":
            try:
                expression = self.display.get()
                result = self._safe_eval(expression)
                self._set_display(result)
            except (ValueError, ZeroDivisionError):
                messagebox.showerror("Error", "Invalid expression")
                self._clear_display()
        else:
            self._append_to_display(char)

if __name__ == "__main__":
    root = tk.Tk()
    calculator = SimpleCalculator(root)
    root.mainloop()
