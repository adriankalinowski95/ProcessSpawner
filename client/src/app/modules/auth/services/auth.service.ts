import { Injectable, OnDestroy } from '@angular/core';
import { BehaviorSubject, catchError, finalize, first, map, Observable, of, retry, switchMap, tap } from 'rxjs';
import { UserDto } from '../models/user-dto';
import { environment } from '../../../../environments/environment.prod';
import { AuthHTTPService } from './auth-http.service';
import { Router } from '@angular/router';
import { shared } from '../../shared/shared';
import { TokenDto } from '../models/token-dto';
import { IsUserAuthValid, UserAuth } from '../models/user-auth';

@Injectable({
  providedIn: 'root'
})
export class AuthService {
    private id: number;
    private authLocalStorageToken = `${environment.appVersion}-${environment.USERDATA_KEY}`;

    currentUserSubject: BehaviorSubject<UserAuth | undefined>;
    isLoadingSubject: BehaviorSubject<boolean>;

    currentUser$: Observable<UserAuth | undefined>;
    isLoading$: Observable<boolean>;
  
    constructor(private authHttpService: AuthHTTPService, private router: Router) { 
        this.isLoadingSubject = new BehaviorSubject<boolean>(false);
        this.currentUserSubject = new BehaviorSubject<UserAuth | undefined>(undefined);
        this.currentUser$ = this.currentUserSubject.asObservable();
        this.isLoading$ = this.isLoadingSubject.asObservable();

        this.id = shared.getRandomNumber();
    }

    get currentUserValue(): UserAuth | undefined {
        console.log("AuthService -> get currentUserValue -> this.id", this.id);
        
        return this.currentUserSubject.value;
    }
    
    set currentUserValue(user: UserAuth) {
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

                } else {
                  this.logout();
                }

                return undefined;
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

    private setAuthFromLocalStorage(userAuth: UserAuth): boolean {
        if (shared.isNotNullOrUndefined(userAuth)) {
            localStorage.setItem(this.authLocalStorageToken, JSON.stringify(userAuth));

            return true;
        }

        return false;
    }

    private setAuthForService(userAuth: UserAuth): boolean {
        this.currentUserSubject.next(userAuth);

        return this.setAuthFromLocalStorage(userAuth);
    }

    private removeAuthForService(): void {
        this.currentUserSubject.next(undefined);
        localStorage.removeItem(this.authLocalStorageToken);
    }
}
