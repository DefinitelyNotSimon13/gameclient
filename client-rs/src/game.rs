use camera::Camera;
use environment::draw_environment;
use player::LocalPlayer;
use raylib::prelude::*;
use raylib::rgui::RaylibDrawGui;
use raylib::{RaylibHandle, RaylibThread};
use state::GameState;
use uuid::Uuid;

use crate::network::tcp::TcpConn;

pub mod camera;
pub mod environment;
pub mod jump;
pub mod player;
pub mod state;

pub struct Game {
    state: GameState,
}

impl Game {
    pub fn init(username: &str, client_id: Uuid, tcp_conn: TcpConn, rl: &mut RaylibHandle) -> Game {
        let player = LocalPlayer::new(username, rl);
        rl.set_target_fps(60);

        Self {
            state: GameState::init(player, client_id, tcp_conn),
        }
    }

    pub fn run(&mut self, mut rl: RaylibHandle, thread: RaylibThread, camera: Camera) {
        let mut in_menu = true;
        while !rl.window_should_close() {
            if in_menu {
                let button_width = 150.0;
                let button_height = 50.0;
                let button_x = (rl.get_screen_width() as f32 - button_width) / 2.0;
                let button_y = (rl.get_screen_height() as f32 - button_height) / 2.0 + 50.0;
                let mut d = rl.begin_drawing(&thread);
                d.clear_background(Color::WHITE);

                if d.gui_button(
                    Rectangle::new(button_x, button_y, button_width, button_height),
                    Some(rstr!("Play")),
                ) {
                    in_menu = false; // Start the game
                }
            } else {
                self.player_mut().update_position(&rl, &camera);
                let mut d = rl.begin_drawing(&thread);
                d.clear_background(Color::WHITE);
                {
                    {
                        let mut d_3d = d.begin_mode3D(camera.camera);

                        draw_environment(&mut d_3d);
                        self.player().draw(&mut d_3d);
                    }

                    d.draw_fps(5, 5);
                    self.player().draw_name(&mut d);
                }
            }
        }
    }

    fn player_mut(&mut self) -> &mut LocalPlayer {
        &mut self.state.local_player
    }

    fn player(&self) -> &LocalPlayer {
        &self.state.local_player
    }
}
