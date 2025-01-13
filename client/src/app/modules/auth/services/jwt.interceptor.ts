import { HttpErrorResponse, HttpEvent, HttpHandler, HttpInterceptor, HttpRequest } from "@angular/common/http";
import { Injectable } from "@angular/core";
import { environment } from "../../../../environments/environment.prod";
import { AuthService } from "./auth.service";
import { catchError, Observable, switchMap, throwError } from "rxjs";
import { shared } from "../../shared/shared";
import { TokenDto } from "../models/token-dto";

@Injectable()
export class JwtInterceptor implements HttpInterceptor {
    private isRefreshing = false;

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
            request = this.addTokenToRequest(request, token.accessToken);
        } 

        return next.handle(request).pipe(
            catchError((error: HttpErrorResponse) => {
                if (error.status === 401) {
                  return this.handle401Error(request, next);
                }

                return throwError(() => error);
            })
        );
    }

    private addTokenToRequest(request: HttpRequest<any>, token: string): HttpRequest<any> {
        return request.clone({
          setHeaders: {
            Authorization: `Bearer ${token}`,
          },
        });
    }

    private handle401Error(
      request: HttpRequest<any>,
      next: HttpHandler
    ): Observable<HttpEvent<any>> {
      if (!this.isRefreshing) {
        this.isRefreshing = true;

        return this.authenticationService.refreshToken().pipe(
          switchMap((token: shared.response.Object<TokenDto>) => {
            this.isRefreshing = false;

            this.authenticationService.setToken(token.object);
            const newRequest = this.addTokenToRequest(request, token.object.accessToken)

            return next.handle(
                newRequest
            );
          }),
          catchError((err) => {
            this.isRefreshing = false;

            this.authenticationService.logout();

            return throwError(() => err);
          })
        );
      } else {
        this.authenticationService.logout();

        return throwError(
          () => new Error('Refreshing token in progress')
        );
      }
    }
}