use color_eyre::owo_colors::OwoColorize;
use raylib::{
    color::Color,
    math::{Vector2, Vector3},
    prelude::RaylibDraw3D,
};

use super::player::Player;

pub fn draw_environment(d: &mut impl RaylibDraw3D) {
    // Ground
    d.draw_plane(
        Vector3::new(0.0, 0.0, 0.0),
        Vector2::new(32.0, 32.0),
        Color::LIGHTGRAY,
    );

    // Walls
    d.draw_cube(Vector3::new(-16.0, 2.5, 0.0), 1.0, 5.0, 32.0, Color::BLUE);
    d.draw_cube(Vector3::new(16.0, 2.5, 0.0), 1.0, 5.0, 32.0, Color::LIME);
    d.draw_cube(Vector3::new(0.0, 2.5, 16.0), 32.0, 5.0, 1.0, Color::GOLD);
    d.draw_cube(Vector3::new(0.0, 2.5, -16.0), 32.0, 5.0, 1.0, Color::ORANGE);
}
