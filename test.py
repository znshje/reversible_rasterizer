import trimesh
from PIL import Image
import numpy as np
import librevras as rr

render_config = rr.RenderConfig()
render_config.vertexColors = False
render_config.vertexColor = [1.0, 0, 0]
render_config.backgroundColor = [0.2, 0.3, 0.4]
render_config.ambientStrength = 0.5
render_config.diffuseStrength = 0.5
render_config.specularStrength = 0.5

camera_config = rr.CameraConfig()
camera_config.width = 1280
camera_config.height = 800
camera_config.R = [-8.428378835009243, 0.6499701799917779, 2.400878313047228]
camera_config.T = [1.7015814557303426, 21.984535530826733, 148.37043107723093]
camera_config.zoom = 13.0

camera_config.R = [69.5069, 3.396862, -1.2601501]
camera_config.T = [10.299490956414884, -139.95694906728178, 59.00354694097169]
camera_config.zoom = 15.682097

camera_config.near = 0.0001
camera_config.far = 10000
print(camera_config)
mesh = trimesh.load('EJWZZZRF_lower.glb', process=False)
verts = []
normals = []
faces = []
add = 0

for gname, g in mesh.geometry.items():
    verts.append(np.asarray(g.vertices))
    normals.append(np.asarray(g.vertex_normals))
    faces.append(np.asarray(g.faces) + add)
    add += len(verts[-1])

verts = np.concatenate(verts, 0)
normals = np.concatenate(normals, 0)
faces = np.concatenate(faces, 0)

instance = rr.init()
rrmesh = rr.Mesh(verts, normals, faces)
print(rrmesh)

id_map = np.array(rr.render_id_map(instance, rrmesh, camera_config, render_config))
print(id_map.shape)
Image.fromarray(id_map.astype(np.uint8)).save('normal.png')

# id_map = np.array(rr.render(instance, rrmesh, camera_config))
# print(id_map.shape)
# id_map = (id_map.astype(np.float32) / np.max(id_map) * 255.0).astype(np.uint8)[:, :, None]
# id_map = np.repeat(id_map, 3, -1)
# Image.fromarray(id_map).save('id_map.png')

id_map = np.array(rr.render(instance, rrmesh, camera_config, render_config))
print(id_map.shape)
Image.fromarray(id_map.astype(np.uint8)).save('render_map.png')

id_map = np.array(rr.render_normal(instance, rrmesh, camera_config, render_config))
print(id_map.shape)
Image.fromarray(id_map.astype(np.uint8)).save('normal_map.png')

id_map = np.array(rr.render_depth(instance, rrmesh, camera_config, render_config))
print(id_map.shape)
Image.fromarray(id_map.astype(np.uint8)).save('depth_map.png')
