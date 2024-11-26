import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ElementListComponent } from './element-list.component';

describe('ElementListComponent', () => {
  let component: ElementListComponent;
  let fixture: ComponentFixture<ElementListComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ElementListComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ElementListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
