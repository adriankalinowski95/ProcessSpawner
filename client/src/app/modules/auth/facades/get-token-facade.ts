import { catchError, first, map, Observable, of, tap } from "rxjs";
import { AuthenticateDto } from "../models/authenticate-dto";
import { AuthHTTPService } from "../services/auth-http.service";
import { ObjectResponse } from "../../shared/response/object-response";
import { TokenDto } from "../models/token-dto";

export class GetTokenFacade {
    constructor(private authHttpService: AuthHTTPService) {}

    getToken(authenticateDto: AuthenticateDto): Observable<ObjectResponse<TokenDto>> {
        return this.authHttpService.login(authenticateDto)
            .pipe(
            first(),
            tap((response: ObjectResponse<TokenDto>) => {
                return of(response);
            }),
            catchError(error => {
                throw new Error("Failed to get token casue: " + error);
            })
        );
    }
}