use std::f32::consts::PI;

const JUMP_SPEED: f32 = 6.0;
const FALL_SPEED: f32 = 6.0;
const MAX_JUMP_HEIGHT: f32 = 3.0;

#[derive(Default)]
pub struct Jump {
    is_jumping: bool,
    is_falling: bool,
    start_height: f32,
    jump_progress: f32,
}

impl Jump {
    pub fn start_jump(&mut self, start_height: f32) {
        if self.is_jumping || self.is_falling {
            return;
        }
        self.is_jumping = true;
        self.jump_progress = 0.0;
        self.start_height = start_height;
    }

    pub fn perform_jump(&mut self, player_y: f32, delta_time: f32) -> f32 {
        self.jump_progress += delta_time * JUMP_SPEED;

        if self.jump_progress >= (PI / 2.0) {
            self.jump_progress = 0.0;
            self.is_jumping = false;
            self.is_falling = true;
            return player_y;
        }

        return self.start_height + MAX_JUMP_HEIGHT * f32::sin(self.jump_progress);
    }

    pub fn perform_fall(&mut self, player_y: f32, delta_time: f32) -> f32 {
        if player_y <= self.start_height {
            self.jump_progress = 0.0;
            self.is_falling = false;
            return self.start_height;
        }

        // Use delta_time to scale the fall progress
        self.jump_progress += delta_time * FALL_SPEED;

        return self.start_height + (MAX_JUMP_HEIGHT * f32::cos(self.jump_progress));
    }

    pub fn is_jumping(&self) -> bool {
        self.is_jumping
    }
    pub fn is_falling(&self) -> bool {
        self.is_falling
    }
}
