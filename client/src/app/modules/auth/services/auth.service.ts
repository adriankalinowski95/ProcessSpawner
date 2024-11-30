import { Injectable, OnDestroy } from '@angular/core';
import { BehaviorSubject, catchError, finalize, first, map, Observable, of, tap } from 'rxjs';
import { UserDto } from '../models/user-dto';
import { environment } from '../../../../environments/environment.prod';
import { SubSink } from 'subsink';
import { AuthHTTPService } from './auth-http.service';
import { Router } from '@angular/router';
import { shared } from '../../shared/shared';

@Injectable({
  providedIn: 'root'
})
export class AuthService implements OnDestroy {
    private subs = new SubSink();
    private authLocalStorageToken = `${environment.appVersion}-${environment.USERDATA_KEY}`;

    currentUserSubject: BehaviorSubject<UserDto | undefined>;
    isLoadingSubject: BehaviorSubject<boolean>;

    currentUser$: Observable<UserDto | undefined>;
    isLoading$: Observable<boolean>;
  
    constructor(private authHttpService: AuthHTTPService, private router: Router) { 
        this.isLoadingSubject = new BehaviorSubject<boolean>(false);
        this.currentUserSubject = new BehaviorSubject<UserDto | undefined>(undefined);
        this.currentUser$ = this.currentUserSubject.asObservable();
        this.isLoading$ = this.isLoadingSubject.asObservable();
    }

    get currentUserValue(): UserDto | undefined{
        return this.currentUserSubject.value;
    }
    
    set currentUserValue(user: UserDto) {
        this.currentUserSubject.next(user);
    }
  
    login(email: string, password: string): Observable<UserDto | undefined> {
        this.isLoadingSubject.next(true);

        return this.authHttpService.login(email, password).pipe(
                first(),
                tap((user: UserDto) => {
                    if (user) {
                        localStorage.setItem(this.authLocalStorageToken, JSON.stringify(user));
                        this.currentUserSubject.next(user);
                    }

                    return user;
                }),
                catchError((err) => {
                    return of(undefined);
                }),
                finalize(() => {
                    this.isLoadingSubject.next(false)
                }));
    }

    logout(): void {    
        localStorage.removeItem(this.authLocalStorageToken);
        this.router.navigate(['/auth/login'], {
          queryParams: {},
        });
    }   

    getUserByToken(): Observable<UserDto | undefined> { 
        const user = this.getAuthFromLocalStorage();
        const auth = user?.getAuth();
        
        if (!shared.isNotNullOrUndefined(user) || !shared.isNotNullOrUndefined(auth)) {
          this.logout();
    
          return of(undefined);
        }
    
        this.isLoadingSubject.next(true);
    
        return this.authHttpService.getUserByToken(auth).pipe(
          map((user: UserDto) => {
            if (user) {
              this.currentUserSubject.next(user);
            } else {
              this.logout();
            }
            return user;
          }),
          finalize(() => this.isLoadingSubject.next(false))
        );
    }

    getAuthFromLocalStorage(): UserDto | undefined {
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

    ngOnDestroy(): void {
        
    }
}
