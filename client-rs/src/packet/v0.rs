use super::{flags::Flags, packet_type::*};
use bytes::{Buf, BufMut, Bytes, BytesMut};
use uuid::Uuid;

#[derive(Debug)]
pub enum Version {
    V0,
    V1,
}

#[derive(Debug)]
pub enum PacketEnum {
    PlayerInit(PacketV1<PlayerInit>),
    SessionInit(PacketV1<SessionInit>),
    ErrorPacket(PacketV1<ErrorPacket>),
}

#[derive(Debug)]
pub struct PacketV1<T: PacketTypeState> {
    pub version: Version, // Byte 0
    pub packet_type: T,   // Byte 1
    pub client_id: Uuid,  // Byte 2 - 17
    pub flags: Flags,     // Byte 18
    pub payload_len: u32,
    pub payload: Vec<u8>,
}

impl<T: PacketTypeState> PacketV1<T> {}

impl PacketV1<PlayerInit> {}
impl PacketV1<SessionInit> {}
impl PacketV1<ErrorPacket> {}
