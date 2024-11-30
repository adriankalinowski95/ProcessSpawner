export class AuthDto {
    readonly accessToken: string;
    readonly refreshToken: string;

    constructor(data: { accessToken: string, refreshToken: string }) {
        this.accessToken = data.accessToken;
        this.refreshToken = data.refreshToken;
    }
}