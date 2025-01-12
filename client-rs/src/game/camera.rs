use raylib::consts::CameraMode;
use raylib::{camera::Camera3D, math::Vector3, RaylibHandle};

const INITIAL_POSITION: Vector3 = Vector3 {
    x: 100.0,
    y: 100.0,
    z: -100.0,
};
const INITIAL_TARGET: Vector3 = Vector3 {
    x: 0.0,
    y: 0.0,
    z: 0.0,
};
const INITIAL_UP: Vector3 = Vector3 {
    x: 0.0,
    y: 1.0,
    z: 0.0,
};
const INITIAL_FOV_Y: f32 = 20.0;

pub struct Camera {
    pub camera: Camera3D,
}

impl Camera {
    pub fn init(rl: &RaylibHandle) -> Self {
        let mut camera =
            Camera3D::perspective(INITIAL_POSITION, INITIAL_TARGET, INITIAL_UP, INITIAL_FOV_Y);

        rl.update_camera(&mut camera, CameraMode::CAMERA_CUSTOM);

        Self { camera }
    }
}
