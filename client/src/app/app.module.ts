import { APP_INITIALIZER, NgModule } from '@angular/core';
import { BrowserModule, provideClientHydration } from '@angular/platform-browser';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { AuthService } from './modules/auth/services/auth.service';
import { provideAnimationsAsync } from '@angular/platform-browser/animations/async';
import { HTTP_INTERCEPTORS, HttpClientModule } from '@angular/common/http';
import { provideAnimations } from '@angular/platform-browser/animations';
import { provideToastr } from 'ngx-toastr';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';
import { MatInputModule } from '@angular/material/input';
import { MatFormFieldModule, MatLabel } from '@angular/material/form-field';
import { MatButtonModule } from '@angular/material/button';
import { MatCardModule } from '@angular/material/card';
import { MatIconModule } from '@angular/material/icon';
import { JwtInterceptor } from './modules/auth/services/jwt.interceptor';

function appInitializer(authService: AuthService) {
    return () => {
        return new Promise((resolve) => {
            //@ts-ignore
            authService.getUserByToken().subscribe().add(resolve);
        });
    };
}

@NgModule({
  declarations: [
    AppComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    NgbModule,
    MatInputModule,
    MatFormFieldModule,
    MatButtonModule,
    MatCardModule,
    MatIconModule,
    MatLabel
  ],
  providers: [
    provideAnimations(),
    AuthService,
    { 
        provide: HTTP_INTERCEPTORS, 
        useClass: JwtInterceptor, 
        multi: true 
    },
    {
        provide: APP_INITIALIZER,
        useFactory: appInitializer,
        multi: true,
        deps: [ AuthService ],
    }
  ],
  bootstrap: [ AppComponent ]
})
export class AppModule { }
