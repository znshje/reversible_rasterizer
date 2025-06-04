import trimesh
from PIL import Image
import numpy as np
from build import librevras as rr

camera_config = rr.CameraConfig()
camera_config.width = 1278
camera_config.height = 798
camera_config.R = [-8.428378835009243, 0.6499701799917779, 2.400878313047228]
camera_config.T = [1.7015814557303426, 21.984535530826733, 148.37043107723093]
camera_config.zoom = 13.0
camera_config.near = 0.0001
camera_config.far = 10000
print(camera_config)
mesh = trimesh.load('0AAQ6BO3_lower.glb', process=False)
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

id_map = np.array(rr.render(instance, rrmesh, camera_config))
print(id_map.shape)
id_map = (id_map.astype(np.float32) / np.max(id_map) * 255.0).astype(np.uint8)[:, :, None]
id_map = np.repeat(id_map, 3, -1)
Image.fromarray(id_map).save('id_map.png')

id_map = np.array(rr.render_normal(instance, rrmesh, camera_config))
print(id_map.shape)
Image.fromarray(id_map.astype(np.uint8)).save('normal_map.png')

id_map = np.array(rr.render_depth(instance, rrmesh, camera_config))
print(id_map.shape)
Image.fromarray(id_map.astype(np.uint8)).save('depth_map.png')
