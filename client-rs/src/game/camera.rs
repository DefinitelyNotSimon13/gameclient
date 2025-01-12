
use raylib::consts::CameraMode;
use raylib::ffi::DEG2RAD;
use raylib::{camera::Camera3D, math::Vector3, RaylibHandle};

const INITIAL_POSITION: Vector3 = Vector3 {
    x: 0.0,
    y: 2.0,
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
            Camera3D::orthographic(INITIAL_POSITION, INITIAL_TARGET, INITIAL_UP, INITIAL_FOV_Y);

        yaw_camera(&mut camera, -45.0 * DEG2RAD as f32, true);
        pitch_camera(&mut camera, -45.0 * DEG2RAD as f32, true, true, false);

        rl.update_camera(&mut camera, CameraMode::CAMERA_CUSTOM);

        Self { camera }
    }
}

fn yaw_camera(camera: &mut Camera3D, angle: f32, rotate_around_target: bool) {
    // based on https://github.com/raysan5/raylib/blob/master/src/rcamera.h
    let up = camera.up;
    let mut target_position = camera.target - camera.position;

    target_position = rotate_around_axis(target_position, up, angle);

    if rotate_around_target {
        camera.position = camera.target - target_position;
    } else {
        camera.target = camera.position + target_position;
    }
}

fn pitch_camera(
    camera: &mut Camera3D,
    mut angle: f32,
    lock_view: bool,
    rotate_around_target: bool,
    rotate_up: bool,
) {
    // based on https://github.com/raysan5/raylib/blob/master/src/rcamera.h
    let up = camera.up;

    let mut target_position = camera.target - camera.position;

    if lock_view {
        let mut max_angle_up = angle_between_vec3(up, target_position);
        max_angle_up -= 0.001;
        if angle > max_angle_up {
            angle = max_angle_up
        }

        let mut max_angle_down =
            angle_between_vec3(Vector3::new(-1.0, -1.0, -1.0) * up, target_position);
        max_angle_down *= -1.0;
        max_angle_down += 0.001;
        if angle < max_angle_down {
            angle = max_angle_down
        }
    }

    let right = get_camera_right(camera);

    target_position = rotate_around_axis(target_position, right, angle);

    if rotate_around_target {
        camera.position = camera.target - target_position;
    } else {
        camera.target = camera.position + target_position;
    }

    if rotate_up {
        camera.up = rotate_around_axis(camera.up, right, angle)
    }
}

fn rotate_around_axis(v: Vector3, mut axis: Vector3, mut angle: f32) -> Vector3 {
    // based on https://github.com/raysan5/raylib/blob/master/src/raymath.h
    let mut result = v;

    // raymath.h does it manually, so I do to - not sure why the normalize function is not used
    let mut length = f32::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    if length == 0.0 {
        length = 1.0
    };
    let i_length = 1.0 / length;
    axis.x *= i_length;
    axis.y *= i_length;
    axis.z *= i_length;

    angle /= 2.0;
    let mut a = f32::sin(angle);
    let b = axis.x * a;
    let c = axis.y * a;
    let d = axis.z * a;
    a = f32::cos(angle);
    let w = Vector3::new(b, c, d);

    // Vector3CrossProduct(w, v)
    let mut wv = Vector3::new(
        w.y * v.z - w.z * v.y,
        w.z * v.x - w.x * v.z,
        w.x * v.y - w.y * v.x,
    );

    // Vector3CrossProduct(w, wv)
    let mut wwv = Vector3::new(
        w.y * wv.z - w.z * wv.y,
        w.z * wv.x - w.x * wv.z,
        w.x * wv.y - w.y * wv.x,
    );

    // Vector3Scale(wv, 2*a)
    a *= 2.0;
    wv.x *= a;
    wv.y *= a;
    wv.z *= a;

    // Vector3Scale(wwv, 2)
    wwv.x *= 2.0;
    wwv.y *= 2.0;
    wwv.z *= 2.0;

    result.x += wv.x;
    result.y += wv.y;
    result.z += wv.z;

    result.x += wwv.x;
    result.y += wwv.y;
    result.z += wwv.z;

    result
}

fn angle_between_vec3(v1: Vector3, v2: Vector3) -> f32 {
    let cross = Vector3::new(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
    );

    let len = f32::sqrt(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z);
    let dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

    (len).atan2(dot)
}

fn get_camera_right(camera: &Camera3D) -> Vector3 {
    let forward = get_camera_forward(camera);
    let up = get_camera_up(camera);

    forward.cross(up).normalized()
}

fn get_camera_forward(camera: &Camera3D) -> Vector3 {
    (camera.target - camera.position).normalized()
}

fn get_camera_up(camera: &Camera3D) -> Vector3 {
    camera.up.normalized()
}
