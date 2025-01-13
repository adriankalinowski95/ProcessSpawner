import { Injectable, OnDestroy } from '@angular/core';
import { BehaviorSubject, catchError, finalize, first, map, Observable, of, retry, switchMap, tap } from 'rxjs';
import { UserDto } from '../models/user-dto';
import { environment } from '../../../../environments/environment.prod';
import { AuthHTTPService } from './auth-http.service';
import { Router } from '@angular/router';
import { shared } from '../../shared/shared';
import { TokenDto } from '../models/token-dto';
import { IsUserAuthValid, UserAuth } from '../models/user-auth';
import { jwtDecode } from "jwt-decode";

interface DecodedToken {
    sub: string;
    exp: number;
    iat: number;
    role?: string[];
}

@Injectable({
  providedIn: 'root'
})
export class AuthService {
    private authLocalStorageToken = `${environment.appVersion}-${environment.USERDATA_KEY}`;
    private userRoles: string[] = [];

    currentUserSubject: BehaviorSubject<UserAuth | undefined>;
    isLoadingSubject: BehaviorSubject<boolean>;

    currentUser$: Observable<UserAuth | undefined>;
    isLoading$: Observable<boolean>;
  
    constructor(private authHttpService: AuthHTTPService, private router: Router) { 
        this.isLoadingSubject = new BehaviorSubject<boolean>(false);
        this.currentUserSubject = new BehaviorSubject<UserAuth | undefined>(undefined);
        this.currentUser$ = this.currentUserSubject.asObservable();
        this.isLoading$ = this.isLoadingSubject.asObservable();

        this.loadAuth();
    }

    getCurrentUserValue(): UserAuth | undefined {
        return this.currentUserSubject.value;
    }
    
    setCurrentUserValue(user: UserAuth) {
        this.currentUserSubject.next(user);
    }

    login(
        email: string, 
        password: string, 
        responseCallback?: (response: shared.response.Object<any>) => void): Observable<UserAuth | undefined> {
        
        this.isLoadingSubject.next(true);

        return this.authHttpService.login({ username:email, password: password }).pipe(
                first(),
                switchMap((tokenResponse: shared.response.Object<TokenDto>) => {
                    if (tokenResponse.status != shared.enums.BaseResponseStatus.Ok) {
                        if (!responseCallback) {
                            return of(undefined);
                        }

                        throw new Error(tokenResponse.errorMessage);
                    }

                    return this.authHttpService.getUserByToken(tokenResponse.object).pipe(
                        first(),
                        map((userResponse: shared.response.Object<UserDto>) => 
                            [tokenResponse, userResponse ] as [shared.response.Object<TokenDto>, shared.response.Object<UserDto>]
                        )
                    );
                }),
                switchMap((response: [shared.response.Object<TokenDto>, shared.response.Object<UserDto>] | undefined) => {
                    if (!response) {
                        return of(undefined);
                    }

                    const [tokenResponse, userResponse] = response;
                    if (userResponse.status != shared.enums.BaseResponseStatus.Ok) {
                        if (!responseCallback) {
                            return of(undefined);
                        }

                        throw new Error(userResponse.errorMessage);
                    }

                    const userAuth: UserAuth = {
                        user: userResponse.object,
                        token: tokenResponse.object
                    };

                    if (!this.setAuthForService(userAuth)) {
                        throw new Error("Error setting user data to local storage");
                    }

                    return of(userAuth);
                }),
                catchError((err) => {
                    if (responseCallback) {
                        responseCallback({
                            errorMessage: err.message, 
                            status: shared.enums.BaseResponseStatus.Error,
                            object: null
                        });
                    }

                    return of(undefined);
                }),
                finalize(() => {
                    this.isLoadingSubject.next(false)
                }));
    }

    logout(): void {    
        this.removeAuthForService();

        this.router.navigate(['/auth/login'], {
          queryParams: {},
        });
    }   

    refreshToken() {
        const auth = this.getAuthFromLocalStorage();
        if (!shared.isNotNullOrUndefined(auth)) {
            this.logout();

            return of(undefined);
        }

        return this.authHttpService.refreshToken(auth.token);
    }

    getUserByToken(): Observable<UserAuth | undefined> { 
        const userAuth = this.getAuthFromLocalStorage();
        if (!shared.isNotNullOrUndefined(userAuth)) {
            this.logout();
        
            return of(undefined);
        }

        if (!IsUserAuthValid(userAuth)) {
            this.logout();
        
            return of(undefined);
        }
    
        this.isLoadingSubject.next(true);
    
        return this.authHttpService.getUserByToken(userAuth.token).pipe(
            map((user: shared.response.Object<UserDto>) => {
                if (user) {
                    const newUserAuth: UserAuth = {
                        user: user.object,
                        token: userAuth.token
                    };

                    if (!this.setAuthFromLocalStorage(newUserAuth)) {
                        throw new Error("Error setting user data to local storage");
                    }
                    
                    return newUserAuth;
                } else {
                  this.logout();
                }

                return undefined;
            }),
            catchError((err) => {
                console.log(err.message);
                
                this.logout();

                return of(undefined);
            }),
            finalize(() => this.isLoadingSubject.next(false))
        );
    }

    getAuthFromLocalStorage(): UserAuth | undefined {
        try {
            const lsValue = localStorage.getItem(this.authLocalStorageToken);
            if (!lsValue) {
                return undefined;
            }

            const authData = JSON.parse(lsValue);
            
            return authData;
        } catch (error) {
            console.error(error);
            
            return undefined;
        }
    }

    getUserRoles(): string[] {
        return this.userRoles;
    }

    setToken(token: TokenDto): boolean {
        var auth = this.getAuthFromLocalStorage();
        if (!shared.isNotNullOrUndefined(auth)) {
            return false;
        }

        auth.token = token;

        this.loadAuth();

        return true;
    }

    private loadAuth() {
        var auth = this.getAuthFromLocalStorage();
        if (!shared.isNotNullOrUndefined(auth)) {
            return;
        }

        this.setAuthForService(auth);
    }

    private setAuthFromLocalStorage(userAuth: UserAuth): boolean {
        if (!shared.isNotNullOrUndefined(userAuth)) {
            return false;
        }
        
        localStorage.setItem(this.authLocalStorageToken, JSON.stringify(userAuth));

        return true;
    }

    private setAuthForService(userAuth: UserAuth): boolean {
        this.currentUserSubject.next(userAuth);
        this.parseTokenAndSetRoles(userAuth.token.accessToken);

        return this.setAuthFromLocalStorage(userAuth);
    }

    private removeAuthForService(): void {
        this.currentUserSubject.next(undefined);
        localStorage.removeItem(this.authLocalStorageToken);
        this.userRoles = [];
    }

    private parseTokenAndSetRoles(token: string): void {
        try {
            const decoded: DecodedToken = jwtDecode(token);
            if (decoded && decoded.role) {
                if (typeof(decoded.role) === 'string') {
                    this.userRoles = [decoded.role];
                    return;
                } else if (typeof(decoded.role) === 'object') {
                    this.userRoles = [];

                    Object.keys(decoded.role).forEach((key) => {
                        if (!decoded.role?.hasOwnProperty(key)) {
                            return;
                        }

                        const role = decoded.role[key as keyof typeof decoded.role] as string;
                        this.userRoles.push(role);
                      });

                    return;
                } else {
                    this.userRoles = [];
                }
            } else {
                this.userRoles = [];
            }
        } catch (error) {
            this.userRoles = [];
        }
    }
}
