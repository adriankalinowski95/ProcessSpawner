import { TokenDto } from "./token-dto";
import { UserDto } from "./user-dto";

export interface UserAuth {
    user: UserDto;
    token: TokenDto;
}

export function IsUserAuthValid(userAuth: UserAuth): boolean {
    return userAuth != null && userAuth.user != null && userAuth.token != null;
}