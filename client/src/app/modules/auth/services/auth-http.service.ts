import { HttpClient, HttpHeaders } from "@angular/common/http";
import { Injectable } from "@angular/core";
import { environment } from "../../../../environments/environment.prod";
import { Observable, of } from "rxjs";
import { TokenDto } from "../models/token-dto";
import { UserDto } from "../models/user-dto";
import { shared } from "../../shared/shared";
import { AuthenticateDto } from "../models/authenticate-dto";

@Injectable({
    providedIn: 'root',
  })
export class AuthHTTPService {
    private USER_URL: string = environment.apiUrl + '/api/User';
    private TOKENS_URL: string = environment.apiUrl + '/api/Tokens';

    private USER_GET_BY_TOKEN_URL: string = this.USER_URL + 'GetByToken';

    constructor(private http: HttpClient) {}

    login(authenticate: AuthenticateDto): Observable<any> {
        return this.http.post(this.TOKENS_URL, authenticate);
    }

    getUserByToken(token: TokenDto): Observable<any> {
        // @Todo change this to a sending request!
        // Right now we are returnig current user from local storage
        return this.http.post(this.USER_GET_BY_TOKEN_URL, null);
    }
}