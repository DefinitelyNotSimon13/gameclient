use bitflags::bitflags;
bitflags! {
    /// Each flag is represented by one bit in a single `u8`.
    #[derive(Default, Debug)]
    pub struct Flags: u8 {
const BIT0 = 1 << 0; // 0x01
  const BIT1 = 1 << 1; // 0x02
        const BIT2 = 1 << 2; // 0x04
        const BIT3 = 1 << 3; // 0x08
        const BIT4 = 1 << 4; // 0x10
        const BIT5 = 1 << 5; // 0x20
        const BIT6 = 1 << 6; // 0x40
        const BIT7 = 1 << 7; // 0x80
    }
}
