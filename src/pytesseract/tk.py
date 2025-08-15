import tkinter as TK
import ctypes
import tkinter.font as font
import os

# Check is programme ran on Windows or Mac
if os.name == 'nt':
    S = '\\'
    iconextension = '.ico'
else:
    S = '/'
    iconextension = '.gif'


def mainloop():
    TK.mainloop()
    
def init(x = 800, y = 600, title = '', icon = f'./files/images/logo{iconextension}', main = False):
    if main:
        window = TK.Tk()
    else:
        window = TK.Toplevel()
    if os.name == 'nt':
        ctypes.windll.shcore.SetProcessDpiAwareness(1)
    window.geometry(str(x) + 'x' + str(y))
    window.grid_columnconfigure(0, weight=1)
    window.title(title)
    window['background'] = '#00000000'

    if os.name == 'nt':
        window.iconbitmap(icon)
    else:
        iconx = TK.PhotoImage(file = icon)
        window.iconphoto(iconx)

    # def fullscreen(event):
    #     window.attributes('-fullscreen', not window.attributes('-fullscreen'))
    #     if window.attributes('-fullscreen'):
    #         # Change window size to display size of computer
    #         screen_width = window.winfo_screenwidth()
    #         screen_height = window.winfo_screenheight()
    #         window.geometry(f'{screen_width}x{screen_height}')
    #     else:
    #         # Change window size to default size
    #         window.geometry(str(x) + 'x' + str(y))
    # window.bind('<F11>', fullscreen)


    # window.iconbitmap(True, iconx)
    # window.attributes("-fullscreen", True)
    # window.bind('<F11>', lambda event: window.attributes('-fullscreen', not window.attributes("-fullscreen")))
    # window.bind('<Escape>', lambda event: window.attributes('-fullscreen', False))
    # window.attributes('-fullscreen', False)
    return window

def wintitle(window, title):
    window.title(title)
    
    
def windowbg(window, file, x = 0, y = 0):
    bg = TK.PhotoImage(file = file)
    label = TK.Label(window, image = bg)
    label.place(x = x, y = y)

def checkbox(window, text = '', x = 0, y = 0, command = None, size = 12):
    def command_wrapper():
        return command()
    checkbox = TK.Checkbutton(window, text = text, font = ('Typewriter', size), bg = '#222222', fg = '#ffffff', activebackground = '#222222', activeforeground = '#ffffff', command = command_wrapper)
    checkbox.place(x = x, y = y)
    return checkbox
    
def label(window, text = '', fg = '#ffffff', bg = '#222222', height = 1, width = 10, x = 0, y = 0, size = 15):
    label = TK.Label(window, text = text, fg = fg, bg = bg, height = height, width = width, font = ('Typewriter', size))
    label.place(x = x, y = y)
    return label
    
def button(window, text = '', width = 10, height = 1, bg = '#222222', fg = '#ffffff', x = 0, y = 0, command = None, size = 12, accelerator = None):
    def command_wrapper():
        return command()
    button = TK.Button(window, text = text, width = width, height = height, bg = bg, fg = fg, command = command_wrapper, font = ('Typewriter', size), accelerator = accelerator, activebackground = '#333333', activeforeground = '#ffffff')
    button.place(x = x, y = y)
    return button

def entry(window, width = 10, x = 0, y = 0):
    entry = TK.Entry(window, width = width, font = ('Typewriter', 10), bg = '#222222', fg = '#ffffff')
    entry.place(x = x, y = y)
    return entry

def canvas(window, width = 10, height = 10, x = 0, y = 0):
    canvas = TK.Canvas(window, width = width, height = height, bg = '#222222', highlightthickness = 0)
    canvas.place(x = x, y = y)
    return canvas

def texteditor(window, width = 10, height = 10, x = 0, y = 0, font = ('Typewriter', 15), scrollbar = True, statusbar = True, linenumber = True, bg = '#222222', fg = '#ffffff'):
    # Create a Text widget and a Scrollbar widget in the window
    texteditor = TK.Text(window, width = width, height = height, bg = bg, fg = fg, font = font, highlightthickness = 0)
    scrollbar = TK.Scrollbar(window)

    # Pack the Scrollbar before the Text widget so it appears on the right
    scrollbar.pack(side='right', fill='y')
    texteditor.pack(side='left', fill='both', expand=True)

    # Associate the Scrollbar with the Text widget
    texteditor.config(yscrollcommand=scrollbar.set)
    scrollbar.config(command=texteditor.yview)
    
    return texteditor

def menu(window, listmenu = {'Menu': [{'Item 1': [None, {'win': ['Enter', '<Return>'], 'mac': ['Enter', '<Return>']}]}, {'Item 2': [None, {'win': ['Enter', '<Return>'], 'mac': ['Enter', '<Return>']}]}]}):
    menu = TK.Menu(window)
    if os.name == 'nt':
        wm = 'win'
    else:
        wm = 'mac'
    for menuitem in listmenu:
        submenu = TK.Menu(menu, tearoff=0, font=('Typewriter', 15))
        for item in listmenu[menuitem]:
            if isinstance(item, dict):  # Check if item is a dictionary
                for itemname in item:
                    if item[itemname][0] != None:
                        submenu.add_command(label=itemname, command=item[itemname][0], font=('Typewriter', 12), accelerator=item[itemname][1][wm][0])  # Add the accelerator parameter
                        window.bind(item[itemname][1][wm][1], lambda event, cmd=item[itemname][0]: cmd())
            else:
                pass
        menu.add_cascade(label=menuitem, menu=submenu)
    window.config(menu=menu)
    return menu

def tab(window, name):
    tab = TK.Frame(window)
    tab.pack(fill = 'both', expand = True)
    return tab

def scrollbar(window, texteditor):
    scrollbar = TK.Scrollbar(window, command = texteditor.yview)
    scrollbar.pack(side = 'right', fill = 'y')
    texteditor.config(yscrollcommand = scrollbar.set)
    return scrollbar

def linenumber(window, texteditor):
    linenumber = TK.Text(window, width = 4, height = 1, bg = '#222222', fg = '#ffffff', font = ('Typewriter', 15))
    linenumber.pack(side = 'left', fill = 'y')
    linenumber.insert('1.0', '1')
    linenumber.config(state = 'disabled')
    return linenumber
    
def statusbar(window, texteditor):
    statusbar = TK.Label(window, text = 'Ln 1, Col 1', bg = '#222222', fg = '#ffffff', font = ('Typewriter', 15))
    statusbar.pack(side = 'bottom', fill = 'x')
    return statusbar

def update(window):
    window.update()
window = None  

