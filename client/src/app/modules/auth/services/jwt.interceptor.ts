import { HttpEvent, HttpHandler, HttpInterceptor, HttpRequest } from "@angular/common/http";
import { Injectable } from "@angular/core";
import { environment } from "../../../../environments/environment.prod";
import { AuthService } from "./auth.service";
import { Observable } from "rxjs";
import { shared } from "../../shared/shared";

@Injectable()
export class JwtInterceptor implements HttpInterceptor {
    constructor(private authenticationService: AuthService) { }

    intercept(request: HttpRequest<any>, next: HttpHandler): Observable<HttpEvent<any>> {
        // add auth header with jwt if user is logged in and request is to the api url
        const user = this.authenticationService.getAuthFromLocalStorage();
        if (!shared.isNotNullOrUndefined(user)) {
            return next.handle(request);
        }

        const token = user.token
        if (!shared.isNotNullOrUndefined(token)) {
            return next.handle(request);
        }
        
        const isLoggedIn = token.accessToken;
        const envUrl = environment.apiUrl;
        const isApiUrl = request.url.startsWith(envUrl);
         if (isLoggedIn && isApiUrl) {
            request = request.clone({
                setHeaders: {
                    Authorization: `Bearer ${token.accessToken}`
                }
            });
        }

        return next.handle(request);
    }
}