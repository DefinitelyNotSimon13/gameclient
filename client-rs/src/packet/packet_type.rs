#[derive(Debug)]
pub struct PlayerInit;
#[derive(Debug)]
pub struct SessionInit;
#[derive(Debug)]
pub struct ErrorPacket;

pub trait PacketTypeState {
    fn name() -> &'static str;
    fn id() -> u8;
}

impl PacketTypeState for PlayerInit {
    fn name() -> &'static str {
        "PlayerInit"
    }

    fn id() -> u8 {
        0
    }
}

impl PacketTypeState for SessionInit {
    fn name() -> &'static str {
        "SessionInit"
    }

    fn id() -> u8 {
        1
    }
}

impl PacketTypeState for ErrorPacket {
    fn name() -> &'static str {
        "Error"
    }

    fn id() -> u8 {
        255
    }
}
