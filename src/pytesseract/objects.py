from PIL import Image, ImageTk
import math
import tkinter as tk
import time
import multitasking

class movement:
    def __init__(self, object):
        self.object = object

    def move(self, location, duration):
        # Move the object to the new coordinates over the specified duration
        start_x, start_y = self.object.x, self.object.y
        end_x, end_y = location
        start_time = time.time()
        @multitasking.task        
        def update_position():
            elapsed = time.time() - start_time
            if elapsed < duration:
                t = elapsed / duration
                self.object.x = start_x + (end_x - start_x) * t
                self.object.y = start_y + (end_y - start_y) * t
                self.object.scene.canvas.after(1, update_position)
            else:
                self.object.x = end_x
                self.object.y = end_y

        update_position()

    def rotate(self, rotation):
        # Directly rotate the object to the new rotation
        self.rotation = rotation

    def move_to_rotation(self, distance, direction):
        # Move in one direction (Locally to the object rotation) for n units
        self.location = (self.location[0] + private.find_sides(
            distance, self.rotation + direction)[0], self.location[1] + private.find_sides(
                distance, self.rotation + direction)[1]
                )

    def move_to_direction(self, distance, direction):
        # Move in one direction (Globally) for n units
        self.location = (self.location[0] + private.find_sides(
            distance, direction)[0], self.location[1] + private.find_sides(
                distance, direction)[1]
                )

class display:
    def __init__(self, object):
        self.data = object

    def display(data, canvas, width, height):
        data.show()
        canvas.create_image(0, 0, image=ImageTk.PhotoImage(data), anchor='nw')
        canvas.update()

    def displaytext(text, canvas, x, y, width, height, font, size):
        canvas.create_text(x, y, text=text, font=(font, size))
        canvas.update()

    def remove_text(canvas):
        canvas.delete('text')
        canvas.update()

    def remove_image(canvas):
        canvas.delete('image')
        canvas.update()

    def change_texture(object, texture):
        object.textureinuse = texture

    def set_size(object, size):
        object.size = size

class sensing:
    def __init__(self, object):
        self.object = object
    
    def touching(self, other):
        scene_width = Image.open(self.object.scene.textures[
                self.object.scene.texture
                ]).size[0]
        scene_height = Image.open(self.object.scene.textures[
                self.object.scene.texture
                ]).size[1]
        width = Image.open(self.object.scene.textures[self.object.texture]).size[0]
        height = Image.open(self.object.scene.textures[self.object.texture]).size[1]
        other_width = Image.open(other.scene.textures[other.texture]).size[0]
        other_height = Image.open(other.scene.textures[other.texture]).size[1]

        # Check if bounding boxes overlap
        if (self.object.x * scene_width / 1000 < other.x * scene_width / 1000 + other_width and
            self.object.x * scene_width / 1000 + width > other.x * scene_width / 1000 and
            self.object.y * scene_height / 1000 < other.y * scene_height / 1000 + other_height and
            self.object.y * scene_height / 1000 + height > other.y * scene_height / 1000):
            return True
        return False
    
    def on_click(self, event):
        # Check if the click is within the object's bounding box
        scene_width = Image.open(self.object.scene.textures[
                self.object.scene.texture
                ]).size[0]
        scene_height = Image.open(self.object.scene.textures[
                self.object.scene.texture
                ]).size[1]
        width = Image.open(self.object.scene.textures[self.object.texture]).size[0]
        height = Image.open(self.object.scene.textures[self.object.texture]).size[1]
        x = event.x
        y = event.y

        if (x > self.object.x * scene_width / 1000 and
            x < self.object.x * scene_width / 1000 + width and
            y > self.object.y * scene_height / 1000 and
            y < self.object.y * scene_height / 1000 + height):
            self.object.clicked_status = True
        else:
            self.object.clicked_status = False

    def distance_to(self, other):
        # Calculate the distance between the two objects
        return math.sqrt((self.object.x - other.x) ** 2 + (self.object.y - other.y) ** 2)

    def clicked(self):
        return self.object.clicked_status

class object:
    def __init__(self, x, y, rotation, scene, texture):
        self.x = x
        self.y = y
        self.rotation = rotation
        self.scene = scene
        self.texture = texture
        self.movement = movement(self)
        self.display = display(self)
        self.sensing = sensing(self)
        self.size = 1
        self.clicked_status = False

    def __new__(cls, x, y, rotation, scene, texture):
        instance = super(object, cls).__new__(cls)
        instance.__init__(x, y, rotation, scene, texture)
        return instance

class private:
    @staticmethod
    def find_sides(hypotenuse, angle_degrees):
        # Convert the angle from degrees to radians
        angle_radians = math.radians(angle_degrees)
        
        # Calculate the opposite side
        opposite = hypotenuse * math.sin(angle_radians)
        
        # Calculate the adjacent side
        adjacent = hypotenuse * math.cos(angle_radians)
        
        return opposite, adjacent