import { AuthDto } from "./auth-dto";

export class UserDto {
    readonly id: number;
    readonly email: string;
    private auth?: AuthDto;

    constructor(data: { id: number, email: string, auth?: AuthDto }) {
        this.id = data.id;
        this.email = data.email;
        this.auth = data.auth;
    }

    setAuth(auth: AuthDto): void {
        this.auth = auth;
    }

    getAuth(): AuthDto | undefined {
        return this.auth;
    }
}