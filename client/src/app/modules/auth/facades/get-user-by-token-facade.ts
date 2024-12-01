import { catchError, first, of, tap } from "rxjs";
import { TokenDto } from "../models/token-dto";
import { AuthHTTPService } from "../services/auth-http.service";
import { UserDto } from "../models/user-dto";
import { ObjectResponse } from "../../shared/response/object-response";

export class GetUserByTokenFacade {
    constructor(private authHttpService: AuthHTTPService) { }

    createGetTokenFacade(token: TokenDto) {
        return this.authHttpService.getUserByToken(token)
            .pipe(
            first(),
            tap((response: ObjectResponse<UserDto>) => {
                return of(response);
            }),
            catchError(error => {
                throw new Error("Failed to get user casue: " + error);
            })
        );
    }
}