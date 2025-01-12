use super::ToByte;

pub mod error_packet;
pub mod player_init;
pub mod session_init;

#[derive(Debug, Default)]
pub struct PlayerInit;
#[derive(Debug, Default)]
pub struct SessionInit;
#[derive(Debug, Default)]
pub struct ErrorPacket;

pub trait PacketTypeState: Default + ToByte {
    const ID: u8;
}

impl<T: PacketTypeState> ToByte for T {
    fn to_byte(&self) -> u8 {
        Self::ID
    }
}

impl PacketTypeState for PlayerInit {
    const ID: u8 = 0;
}

impl PacketTypeState for SessionInit {
    const ID: u8 = 1;
}

impl PacketTypeState for ErrorPacket {
    const ID: u8 = 255;
}
