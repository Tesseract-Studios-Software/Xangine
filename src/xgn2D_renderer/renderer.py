from PIL import Image, ImageTk # type: ignore
import objects
import scenes

def render(scene):
    # Load all the textures
    objecttextures = {}
    for object in scene.objects:
        objecttextures[object] = render_object(scene.objects[object])

    scenetexture = Image.open(scene.textures[scene.texture])

    # Create a new image with scene and object textures
    image = Image.new('RGB', (scenetexture.width, scenetexture.height))
    image.paste(scenetexture, (0, 0))
    # Turn object coordinates into pixel coordinates
    for object in scene.objects:
        image.paste(objecttextures[object], (
            round(scene.objects[object].x * scenetexture.width / 1000), 
            round(scene.objects[object].y * scenetexture.height / 1000)
            )
        )
        
    return image

def render_object(object):
    '''Load an object texture.\n
    You do not need to run this, this is a private function'''
    texture = object.scene.textures[object.texture]
    image = Image.open(texture)
    image = image.resize((image.width * object.size, image.height * object.size))
    return image