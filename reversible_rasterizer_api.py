import reversible_rasterizer as rr
import numpy as np


class ReversibleRasterizer:
    def __init__(self):
        self.camera_config = rr.CameraConfig()
        self.mesh = None

    def set_camera(self, width: int, height: int, R: list = None, T: list = None, zoom: float = 1.0, near: float = 1e-4,
                   far: float = 1e4) -> 'ReversibleRasterizer':
        """
        Set orthographic camera parameters

        :param width: render width
        :param height: render height
        :param R: euler rotation in degrees
        :param T: translation in mm
        :param zoom: zoom factor
        :param near: near clip plane
        :param far: far clip plane
        :return:
        """
        if T is None:
            T = [0, 0, 0]
        if R is None:
            R = [1, 0, 0]
        self.camera_config.width = width
        self.camera_config.height = height
        self.camera_config.R = R
        self.camera_config.T = T
        self.camera_config.zoom = zoom
        self.camera_config.near = near
        self.camera_config.far = far
        return self

    def set_mesh(self, vertices: np.ndarray, normals: np.ndarray, faces: np.ndarray) -> 'ReversibleRasterizer':
        """
        Set the mesh data for the object.

        :param vertices: A numpy array of 3D vertices with shape (N, 3).
        :param normals: A numpy array of corresponding vertex normals with shape (N, 3).
        :param faces: A numpy array of faces, where each face is a list of vertex indices with shape (M, 3).
        """
        self.mesh = rr.Mesh(vertices, normals, faces)
        return self

    def render(self) -> np.ndarray:
        """
        Render the mesh using the current camera configuration.

        :return: A numpy array of the rendered image, where each element/pixel represents the corresponding the face index of mesh.
        """
        return np.array(rr.render(self.mesh, self.camera_config))
