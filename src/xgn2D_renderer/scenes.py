import tkinter

class scene:
    def __init__(self, objects, texture, textures):
        self.objects = objects
        self.texture = texture
        self.canvas = None
        self.textures = textures
        
    def __new__(cls, objects, texture, textures):
        instance = super(scene, cls).__new__(cls)
        instance.__init__(objects, texture, textures)
        return instance

    def bind_mouse_events(self, canvas):
        canvas.bind('<Motion>', self.update_mouse_coords)
        self.mouse_x = 0
        self.mouse_y = 0

    def update_mouse_coords(self, event):
        self.mouse_x = event.x
        self.mouse_y = event.y