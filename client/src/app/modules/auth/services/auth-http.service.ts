import { HttpClient, HttpHeaders } from "@angular/common/http";
import { Injectable } from "@angular/core";
import { environment } from "../../../../environments/environment.prod";
import { Observable, of } from "rxjs";
import { AuthDto } from "../models/auth-dto";
import { UserDto } from "../models/user-dto";
import { shared } from "../../shared/shared";
import { Authenticate } from "../models/authenticate";

@Injectable({
    providedIn: 'root',
  })
export class AuthHTTPService {
    private USER_URL: string = environment.apiUrl + '/api/User';
    private TOKENS_URL: string = environment.apiUrl + '/api/Tokens';

    private USER_GET_BY_TOKEN_URL: string = this.USER_URL + 'GetByToken';

    constructor(private http: HttpClient) {}

    login(email: string, password: string): Observable<any> {
        const notFoundError = new Error('Not Found');
        if (!email || !password) {
            return of(notFoundError);
        }

        const authenticate: Authenticate = ({ 
            username:email, password:password}
        );

        return this.http.post<AuthDto>(this.TOKENS_URL, authenticate);
    }

    getUserByToken(token: AuthDto): Observable<any> {
        const notFoundError = new Error('Not Found');
        if (!shared.isNotNullOrUndefined(token) || shared.isNotNullOrUndefined(token.accessToken) ) {
            return of(notFoundError);
        }
        
        // @Todo change this to a sending request!
        // Right now we are returnig current user from local storage
        return this.http.post<UserDto>(this.USER_GET_BY_TOKEN_URL, null);
    }
}