using System;
namespace Shared.Types.Interfaces {
    public interface IUser {
        public int Id { get; set; }
        public string? Username { get; set; }
        public string Password { get; set; }
        public string? Firstname { get; set; }
        public string? Lastname { get; set; }
        public string Email { get; set; }

        public int UserBotCommandsId { get; set; }
        public int UserSettingsId { get; set; }
    }
}