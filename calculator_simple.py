import tkinter as tk
from tkinter import font, messagebox

class SimpleCalculator:
    def __init__(self, root):
        self.root = root
        self.root.title("Simple Calculator")
        self.root.geometry("400x500")
        self.root.resizable(False, False)

        # Display
        self.display = tk.Entry(
            root,
            font=("Arial", 24),
            borderwidth=2,
            relief="solid",
            justify="right"
        )
        self.display.pack(fill="both", padx=10, pady=20, ipady=20)

        # Buttons
        button_frame = tk.Frame(root)
        button_frame.pack(fill="both", expand=True, padx=10, pady=10)

        buttons = [
            ["7", "8", "9", "/"],
            ["4", "5", "6", "*"],
            ["1", "2", "3", "-"],
            ["0", ".", "=", "+"],
            ["C"]
        ]

        self.button_font = font.Font(size=18, weight="bold")

        for row_idx, row in enumerate(buttons):
            for col_idx, btn_text in enumerate(row):
                btn = tk.Button(
                    button_frame,
                    text=btn_text,
                    font=self.button_font,
                    command=lambda x=btn_text: self.on_button_click(x)
                )

                if btn_text == "C":
                    btn.grid(row=row_idx, column=0, columnspan=4, sticky="nsew", padx=5, pady=5)
                else:
                    btn.grid(row=row_idx, column=col_idx, sticky="nsew", padx=5, pady=5)

                button_frame.grid_rowconfigure(row_idx, weight=1)
                button_frame.grid_columnconfigure(col_idx, weight=1)

    def on_button_click(self, char):
        if char == "C":
            self.display.delete(0, tk.END)
        elif char == "=":
            try:
                expression = self.display.get()
                result = eval(expression)
                self.display.delete(0, tk.END)
                self.display.insert(0, str(result))
            except:
                messagebox.showerror("Error", "Invalid expression")
                self.display.delete(0, tk.END)
        else:
            self.display.insert(tk.END, char)

if __name__ == "__main__":
    root = tk.Tk()
    calculator = SimpleCalculator(root)
    root.mainloop()
