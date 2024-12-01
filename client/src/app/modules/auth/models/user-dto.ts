import { shared } from "../../shared/shared";
import { TokenDto } from "./token-dto";

export class UserDto {
    readonly id: number;
    readonly email: string;
    private token?: TokenDto;

    constructor(data: { id: number, email: string, token?: TokenDto }) {
        this.id = data.id;
        this.email = data.email;
        this.token = data.token;
    }

    setToken(token: TokenDto): void {
        this.token = token;
    }

    getToken(): TokenDto | undefined {
        return this.token;
    }

    static IsValid(data: any): data is UserDto {
        if (!shared.isNotNullOrUndefined(data)) {
            return false;
        }

        const isIdField = 'id' in data;
        const isEmailField = 'email' in data;
        const isTokenFiled = 'token' in data;
        
        return isIdField && isEmailField && isTokenFiled;
    }
}

