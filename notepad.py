#!/usr/bin/env python3
"""Simple Notepad Application"""

import tkinter as tk
from tkinter import filedialog, messagebox
import os

class SimpleNotepad:
    def __init__(self, root):
        self.root = root
        self.root.title("Simple Notepad")
        self.root.geometry("800x600")

        self.filename = None

        # Create menu bar
        self.create_menu()

        # Create text area
        self.text_area = tk.Text(root, wrap=tk.WORD, undo=True)
        self.text_area.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        # Status bar
        self.status_var = tk.StringVar()
        self.status_var.set("새 파일")
        self.status_bar = tk.Label(root, textvariable=self.status_var, bd=1, relief=tk.SUNKEN, anchor=tk.W)
        self.status_bar.pack(side=tk.BOTTOM, fill=tk.X)

    def create_menu(self):
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)

        # File menu
        file_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="파일", menu=file_menu)
        file_menu.add_command(label="새 파일", command=self.new_file)
        file_menu.add_command(label="열기", command=self.open_file)
        file_menu.add_command(label="저장", command=self.save_file)
        file_menu.add_command(label="다른 이름으로 저장", command=self.save_as_file)
        file_menu.add_separator()
        file_menu.add_command(label="종료", command=self.root.quit)

        # Edit menu
        edit_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="편집", menu=edit_menu)
        edit_menu.add_command(label="실행 취소", command=lambda: self.text_area.edit_undo())
        edit_menu.add_command(label="다시 실행", command=lambda: self.text_area.edit_redo())
        edit_menu.add_separator()
        edit_menu.add_command(label="잘라내기", command=self.cut)
        edit_menu.add_command(label="복사", command=self.copy)
        edit_menu.add_command(label="붙여넣기", command=self.paste)

        # Help menu
        help_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="도움말", menu=help_menu)
        help_menu.add_command(label="정보", command=self.show_about)

    def new_file(self):
        if self.text_area.get(1.0, tk.END).strip():
            response = messagebox.askyesno("확인", "저장하지 않은 내용이 있습니다. 계속하시겠습니까?")
            if not response:
                return
        self.text_area.delete(1.0, tk.END)
        self.filename = None
        self.status_var.set("새 파일")
        self.root.title("Simple Notepad")

    def open_file(self):
        filename = filedialog.askopenfilename(
            defaultextension=".txt",
            filetypes=[("텍스트 파일", "*.txt"), ("모든 파일", "*.*")]
        )
        if filename:
            try:
                with open(filename, 'r', encoding='utf-8') as file:
                    content = file.read()
                self.text_area.delete(1.0, tk.END)
                self.text_area.insert(1.0, content)
                self.filename = filename
                self.status_var.set(f"파일: {os.path.basename(filename)}")
                self.root.title(f"Simple Notepad - {os.path.basename(filename)}")
            except Exception as e:
                messagebox.showerror("오류", f"파일을 열 수 없습니다: {e}")

    def save_file(self):
        if self.filename:
            try:
                content = self.text_area.get(1.0, tk.END)
                with open(self.filename, 'w', encoding='utf-8') as file:
                    file.write(content)
                self.status_var.set(f"저장됨: {os.path.basename(self.filename)}")
                messagebox.showinfo("성공", "파일이 저장되었습니다.")
            except Exception as e:
                messagebox.showerror("오류", f"파일을 저장할 수 없습니다: {e}")
        else:
            self.save_as_file()

    def save_as_file(self):
        filename = filedialog.asksaveasfilename(
            defaultextension=".txt",
            filetypes=[("텍스트 파일", "*.txt"), ("모든 파일", "*.*")]
        )
        if filename:
            try:
                content = self.text_area.get(1.0, tk.END)
                with open(filename, 'w', encoding='utf-8') as file:
                    file.write(content)
                self.filename = filename
                self.status_var.set(f"저장됨: {os.path.basename(filename)}")
                self.root.title(f"Simple Notepad - {os.path.basename(filename)}")
                messagebox.showinfo("성공", "파일이 저장되었습니다.")
            except Exception as e:
                messagebox.showerror("오류", f"파일을 저장할 수 없습니다: {e}")

    def cut(self):
        self.text_area.event_generate("<<Cut>>")

    def copy(self):
        self.text_area.event_generate("<<Copy>>")

    def paste(self):
        self.text_area.event_generate("<<Paste>>")

    def show_about(self):
        messagebox.showinfo("정보", "Simple Notepad v1.0\n\n간단한 텍스트 에디터")

if __name__ == "__main__":
    root = tk.Tk()
    app = SimpleNotepad(root)
    root.mainloop()
