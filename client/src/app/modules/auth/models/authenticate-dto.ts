export class AuthenticateDto {
    readonly username: string;
    readonly password: string;

    constructor(data: { username: string, password: string }) {
        this.username = data.username;
        this.password = data.password;
    }
}